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
	using CallBackFunction = void(*)(CallBackArgs ...);
	using CallBackVec = std::vector<CallBackFunction>;
	
private:
	CallBackVec mCallBacksVec{};

public:
	Delegate() = default;

	void Add(const CallBackFunction& func)
	{
		mCallBacksVec.push_back(func);
	}


	void InvokeAll(CallBackArgs ... args)
	{
		for (auto& f : mCallBacksVec)
		{
			std::invoke(f , args ...);
		}
	}

	
};


