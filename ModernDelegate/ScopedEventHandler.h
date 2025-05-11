#pragma once
#include <optional>
#include "EventManager.h"


class ScopedEventHandler
{
	std::optional<size_t> mCallBackID{};
	std::string mEventName{};
public:

	template<typename ... CallbackArgs>
	explicit ScopedEventHandler(const std::string& eventName, void(*func)(CallbackArgs ...))
		:
		mEventName(eventName)
	{
		auto eventManager = EventManager::GetEventManager();
		if (eventManager)
		{
			mCallBackID = eventManager->AttachToEvent(mEventName, func);
		}
	}

	template <typename T, typename... CallbackArgs>
	explicit ScopedEventHandler(const std::string& eventName, T* obj, void (T::* method)(CallbackArgs...))
		:
		mEventName(eventName)
	{
		auto eventManager = EventManager::GetEventManager();
		if (eventManager)
		{
			mCallBackID = eventManager->AttachToEvent(mEventName, obj, method);
		}
	}

	void Detach()
	{
		auto eventManager = EventManager::GetEventManager();
		if (eventManager)
		{
			eventManager->DetachFromEvent(mEventName, mCallBackID.value());
			mEventName = "";
			mCallBackID = std::nullopt;
		}
	}

	[[nodiscard]] bool IsInitialized() const
	{
		return mCallBackID.has_value();
	}

	~ScopedEventHandler()
	{
		if (IsInitialized())
		{
			Detach();
		}
	}
};