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
		if (mEventMap.contains(nameEvent))
		{
			// LOG_ERROR("Event with ", nameEvent, " allready is created");
			return;
		}

		mEventMap[nameEvent] = std::make_unique<Delegate<CallbackArgs ...>>();
	}

	template<typename ... CallbackArgs>
	void AttachToEvent(const std::string& nameEvent, void(*func)(CallbackArgs ...))
	{
		if (mEventMap.contains(nameEvent))
		{
			auto delegate = static_cast<Delegate<CallbackArgs ...>*>(mEventMap.at(nameEvent).get());
			delegate->Add(func);
		}
	}

	template <typename T, typename... CallbackArgs>
	void AttachToEvent(const std::string& nameEvent, T* obj, void (T::* method)(CallbackArgs...))
	{
		if (mEventMap.contains(nameEvent))
		{
			auto delegate = static_cast<Delegate<CallbackArgs...>*>(mEventMap.at(nameEvent).get());
			delegate->Add(obj, method);
		}
	}

	template<typename ... CallbackArgs>
	void CallAllEventSubscribes(const std::string& nameEvent, CallbackArgs ... args)
	{
		if (mEventMap.contains(nameEvent))
		{
			auto delegate = static_cast<Delegate<CallbackArgs ...>*>(mEventMap.at(nameEvent).get());
			delegate->InvokeAll(args ...);
		}
	}

};