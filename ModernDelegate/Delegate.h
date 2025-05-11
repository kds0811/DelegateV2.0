#pragma once
#include <functional>
#include <vector>

class IDelegate
{
public:
	IDelegate() = default;
	virtual ~IDelegate() = default;
};

template <typename ... CallBackArgs>
class Delegate : public IDelegate
{
public:
	using CallBackFunction = std::function<void(CallBackArgs ...)>;
	using CallBackVec = std::vector<CallBackFunction>;
	
private:
	CallBackVec mCallBacksVec{};

public:
	Delegate() = default;

	void Add(const CallBackFunction& func)
	{
		mCallBacksVec.push_back(func);
	}

	template <typename T>
	void Add(T* obj, void (T::* method)(CallBackArgs...))
	{
		mCallBacksVec.push_back([obj, method](CallBackArgs... args) {
			(obj->*method)(std::forward<CallBackArgs>(args) ...);
			});
	}

	void InvokeAll(CallBackArgs ... args)
	{
		for (auto& f : mCallBacksVec)
		{
			std::invoke(f , std::forward<CallBackArgs>(args) ...);
		}
	}
};


