#pragma once
#include <string>
#include <unordered_map>
#include "Delegate.h"
#include <memory>

class EventManager
{
	using EventMap = std::unordered_map<std::string, std::unique_ptr<IDelegate>>;

	EventMap mEventMap;

public:

	template<typename ... CallbackArgs>
	void CreateEvent(const std::string& nameEvent)
	{
		if (!CheckEventContains(nameEvent))
		{
			mEventMap[nameEvent] = std::make_unique<Delegate<CallbackArgs ...>>();
		}
		else
		{
			// LOG_ERROR("Event with ", nameEvent, " allready is created");
		}
	}

	template<typename ... CallbackArgs>
	void AttachToEvent(const std::string& nameEvent, void(*func)(CallbackArgs ...))
	{
		if (CheckEventContains(nameEvent))
		{
			auto delegate = static_cast<Delegate<CallbackArgs ...>*>(mEventMap.at(nameEvent).get());
			delegate->Add(func);
		}
	}

	template <typename T, typename... CallbackArgs>
	void AttachToEvent(const std::string& nameEvent, T* obj, void (T::* method)(CallbackArgs...))
	{
		if (CheckEventContains(nameEvent))
		{
			auto delegate = static_cast<Delegate<CallbackArgs...>*>(mEventMap.at(nameEvent).get());
			delegate->Add(obj, method);
		}
	}

	template<typename ... CallbackArgs>
	void CallAllEventSubscribes(const std::string& nameEvent, CallbackArgs&& ... args)
	{
		if (CheckEventContains(nameEvent))
		{
			auto delegate = static_cast<Delegate<CallbackArgs ...>*>(mEventMap.at(nameEvent).get());
			delegate->InvokeAll(std::forward<CallbackArgs>(args) ...);
		}
	}

private:
	bool CheckEventContains(const std::string& nameEvent)
	{
		if (!mEventMap.contains(nameEvent))
		{
			// LOG_ERROR("An event named  ", nameEvent, "  doesn't exist");
			return false;
		}
		return true;
	}

};