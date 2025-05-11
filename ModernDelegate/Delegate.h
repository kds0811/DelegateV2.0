#pragma once
#include <functional>
#include <vector>
#include <unordered_map>

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
	using typeID = size_t;
	using CallBackFunction = std::function<void(CallBackArgs ...)>;
	using CallBackMap = std::unordered_map<typeID, CallBackFunction>;

private:
	CallBackMap mCallBackMap{};
	typeID mID = 0;

public:
	Delegate() = default;

	void Attach(const CallBackFunction& func)
	{
		mCallBackMap[GetID()] = func;
	}

	template <typename T>
	void Attach(T* obj, void (T::* method)(CallBackArgs...))
	{
		mCallBackMap[GetID()] = [obj, method](CallBackArgs... args) {
			(obj->*method)(std::forward<CallBackArgs>(args) ...); };
	}

	template <typename ... CallbackArgs>
	void InvokeAll(CallBackArgs&& ... args)
	{
		for (const auto& [id, func] : mCallBackMap)
		{
			std::invoke(func, std::forward<CallBackArgs>(args)...);
		}
	}
private:
	typeID GetID()
	{
		return mID++;
	}
};


