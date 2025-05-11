#pragma once
#include <string>
#include <unordered_map>
#include "Delegate.h"
#include <memory>
#include <optional>

class EventManager
{
	using EventMap = std::unordered_map<std::string, std::unique_ptr<IDelegate>>;
	EventMap mEventMap;

public:
	template<typename ... CallbackArgs>
	void CreateEvent(const std::string& nameEvent);

	template<typename ... CallbackArgs>
	[[nodiscard]] std::optional<size_t> AttachToEvent(const std::string& nameEvent, void(*func)(CallbackArgs ...));


	template <typename T, typename... CallbackArgs>
	[[nodiscard]] std::optional<size_t> AttachToEvent(const std::string& nameEvent, T* obj, void (T::* method)(CallbackArgs...));


	template<typename ... CallbackArgs>
	void DetachFromEvent(const std::string& nameEvent, size_t callBackID);


	template<typename ... CallbackArgs>
	void InvokeAllSubscribers(const std::string& nameEvent, CallbackArgs&& ... args);


	template<typename ... CallbackArgs>
	void ClearEvent(const std::string& nameEvent);


	template<typename ... CallbackArgs>
	[[nodiscard]] std::optional<bool> IsEventEmpty(const std::string& nameEvent);


private:
	[[nodiscard]] inline bool HasEvent(const std::string& nameEvent);

	template<typename ...CallbackArgs>
	[[nodiscard]] Delegate<CallbackArgs...>* GetDelegate(const std::string& nameEvent);


};

template<typename ...CallbackArgs>
inline void EventManager::CreateEvent(const std::string& nameEvent)
{
	if (!HasEvent(nameEvent))
	{
		mEventMap[nameEvent] = std::make_unique<Delegate<CallbackArgs ...>>();
	}
	else
	{
		// LOG_ERROR("Event with ", nameEvent, " allready is created");
	}
}

template<typename ...CallbackArgs>
inline std::optional<size_t> EventManager::AttachToEvent(const std::string& nameEvent, void(*func)(CallbackArgs...))
{
	auto delegate = GetDelegate<CallbackArgs...>(nameEvent);
	if (delegate)
	{
		return delegate->Attach(func);
	}
	return std::nullopt;
}

template<typename T, typename ...CallbackArgs>
inline std::optional<size_t> EventManager::AttachToEvent(const std::string& nameEvent, T* obj, void(T::* method)(CallbackArgs...))
{
	auto delegate = GetDelegate<CallbackArgs...>(nameEvent);
	if (delegate)
	{
		return delegate->Attach(obj, method);
	}
	return std::nullopt;
}

template<typename ...CallbackArgs>
inline void EventManager::DetachFromEvent(const std::string& nameEvent, size_t callBackID)
{
	auto delegate = GetDelegate<CallbackArgs...>(nameEvent);
	if (delegate)
	{
		delegate->Detach(callBackID);
	}
}

template<typename ...CallbackArgs>
inline void EventManager::InvokeAllSubscribers(const std::string& nameEvent, CallbackArgs && ...args)
{
	auto delegate = GetDelegate<CallbackArgs...>(nameEvent);
	if (delegate && !delegate->IsEmpty())
	{
		delegate->InvokeAll(std::forward<CallbackArgs>(args)...);
	}
}

template<typename ...CallbackArgs>
inline void EventManager::ClearEvent(const std::string& nameEvent)
{
	auto delegate = GetDelegate<CallbackArgs...>(nameEvent);
	if (delegate)
	{
		delegate->Clear();
	}
}

template<typename ...CallbackArgs>
inline std::optional<bool> EventManager::IsEventEmpty(const std::string& nameEvent)
{
	auto delegate = GetDelegate<CallbackArgs...>(nameEvent);
	if (delegate)
	{
		return delegate->IsEmpty();
	}
	return std::nullopt;
}

template<typename ...CallbackArgs>
inline Delegate<CallbackArgs...>* EventManager::GetDelegate(const std::string& nameEvent)
{
	if (!HasEvent(nameEvent))
	{
		return nullptr;
	}
	return static_cast<Delegate<CallbackArgs...>*>(mEventMap.at(nameEvent).get());
}

inline bool EventManager::HasEvent(const std::string& nameEvent)
{
	if (!mEventMap.contains(nameEvent))
	{
		// LOG_ERROR("An event named  ", nameEvent, "  doesn't exist");
		return false;
	}
	return true;
}