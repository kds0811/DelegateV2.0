#pragma once
#include <functional>
#include <vector>

template <typename ... CallBackArgs>
class Delegate
{
	using CallBackFunction = void(*)(CallBackArgs ...);
	using CallBackVec = std::vector<CallBackFunction>;

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


