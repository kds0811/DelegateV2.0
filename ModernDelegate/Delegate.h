#pragma once
#include <functional>
#include <vector>




class Delegate
{
	using CallBack = std::function<void()>;
	std::vector<CallBack> mCallBacks;

public:
	Delegate() = default;

	void Add(const CallBack& func)
	{
		mCallBacks.push_back(func);
	}

	void Invoke()
	{
		for (auto& f : mCallBacks)
		{
			f();
		}
	}
};


