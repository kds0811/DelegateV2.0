#pragma once
#include "Delegate.h"
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>


namespace Delegate
{
  namespace Details
  {
    class ScopedEventHandler;

    class EventManager
    {
      friend ScopedEventHandler;

      using EventMap = std::unordered_map<std::string_view, std::unique_ptr<BaseDelegate>>;

      EventMap mEventMap; // storage

    public:
      ~EventManager() {}
      [[nodiscard]] static inline EventManager* Get()
      {
        static EventManager* instance = new EventManager();
        return instance;
      }

      template <typename... CallbackArgs>
      void CreateEvent(std::string_view eventName);

      template <typename... CallbackArgs>
      void InvokeAllEventSubscribers(std::string_view eventName, CallbackArgs&&... args);

      template <typename... CallbackArgs>
      void ClearEvent(std::string_view eventName);

      template <typename... CallbackArgs>
      [[nodiscard]] std::optional<bool> IsEventEmpty(std::string_view eventName);

    private:
      EventManager() {}
      EventManager(const EventManager&)                = delete;
      EventManager(EventManager&&) noexcept            = delete;
      EventManager& operator=(const EventManager&)     = delete;
      EventManager& operator=(EventManager&&) noexcept = delete;


      template <typename... CallbackArgs>
      [[nodiscard]] std::int32_t AttachToEvent(std::string_view eventName, void (*func)(CallbackArgs...));

      template <typename T, typename... CallbackArgs>
      [[nodiscard]] std::int32_t AttachToEvent(std::string_view eventName, T* obj, void (T::*method)(CallbackArgs...));

      template <typename... CallbackArgs>
      void DetachFromEvent(std::string_view eventName, std::int32_t callBackID);

      [[nodiscard]] inline bool HasEvent(std::string_view eventName);

      template <typename... CallbackArgs>
      [[nodiscard]] Delegate<CallbackArgs...>* GetDelegate(std::string_view eventName);
    };

    template <typename... CallbackArgs>
    inline void EventManager::CreateEvent(std::string_view eventName)
    {
      if (HasEvent(eventName))
        return;

      auto dg = std::make_unique<Delegate::Delegate<CallbackArgs...>>();
      mEventMap.emplace(eventName, std::move(dg));
    }

    template <typename... CallbackArgs>
    inline std::int32_t EventManager::AttachToEvent(std::string_view eventName, void (*func)(CallbackArgs...))
    {
      auto delegate = GetDelegate<CallbackArgs...>(eventName);
      if (delegate)
      {
        return delegate->Attach(func);
      }
      else
      {
        CreateEvent(eventName);
        auto dg = GetDelegate<CallbackArgs...>(eventName);
        return dg->Attach(func);
      }
    }

    template <typename T, typename... CallbackArgs>
    inline std::int32_t EventManager::AttachToEvent(
        std::string_view eventName,
        T* obj,
        void (T::*method)(CallbackArgs...))
    {
      auto delegate = GetDelegate<CallbackArgs...>(eventName);
      if (delegate)
      {
        return delegate->Attach(obj, method);
      }
      else
      {
        CreateEvent(eventName);
        auto dg = GetDelegate<CallbackArgs...>(eventName);
        return dg->Attach(obj, method);
      }
    }

    template <typename... CallbackArgs>
    inline void EventManager::DetachFromEvent(std::string_view eventName, std::int32_t callBackID)
    {
      auto delegate = GetDelegate<CallbackArgs...>(eventName);
      if (delegate)
      {
        delegate->Detach(callBackID);
      }
    }

    template <typename... CallbackArgs>
    inline void EventManager::InvokeAllEventSubscribers(std::string_view eventName, CallbackArgs&&... args)
    {
      auto delegate = GetDelegate<CallbackArgs...>(eventName);
      if (delegate && !delegate->IsEmpty())
      {
        delegate->InvokeAll(std::forward<CallbackArgs>(args)...);
      }
      else
      {
        // LOG_ERROR("An event named  ", nameEvent, "  doesn't exist");
      }
    }

    template <typename... CallbackArgs>
    inline void EventManager::ClearEvent(std::string_view eventName)
    {
      auto delegate = GetDelegate<CallbackArgs...>(eventName);
      if (delegate)
      {
        delegate->Clear();
      }
    }

    template <typename... CallbackArgs>
    inline std::optional<bool> EventManager::IsEventEmpty(std::string_view nameEvent)
    {
      auto delegate = GetDelegate<CallbackArgs...>(nameEvent);
      if (delegate)
      {
        return delegate->IsEmpty();
      }
      return std::nullopt;
    }

    template <typename... CallbackArgs>
    inline Delegate<CallbackArgs...>* EventManager::GetDelegate(std::string_view nameEvent)
    {
      if (!HasEvent(nameEvent))
      {
        return nullptr;
      }
      return static_cast<Delegate<CallbackArgs...>*>(mEventMap.at(nameEvent.data()).get());
    }

    inline bool EventManager::HasEvent(std::string_view nameEvent)
    {
      if (!mEventMap.contains(nameEvent))
      {
        // LOG_ERROR("An event named  ", nameEvent, "  doesn't exist");
        return false;
      }
      return true;
    }

  } // namespace Details

} // namespace Delegate

    /**
 * The `EventManager` class is responsible for managing events and their associated subscribers in an event-driven system.
 * 
 * Key Responsibilities:
 * - Manages the lifecycle of named events, including creation, invocation, and cleanup.
 * - Provides methods to invoke all subscribers of an event, clear events, and check if an event exists or is empty.
 * - Ensures that event subscriptions (attaching/detaching handlers) are managed exclusively through the `ScopedEventHandler` class.
 *   Direct access to subscription methods (`AttachToEvent` and `DetachFromEvent`) is restricted to `ScopedEventHandler` via a `friend` relationship.
 * - Implements a singleton pattern to provide global access to the event management system.
 * 
 * Usage:
 * - Events are created and managed internally by the `EventManager`.
 * - Subscriptions to events must be performed using the `ScopedEventHandler` class, which ensures proper resource management and automatic detachment of handlers.
 * - Event invocation and other operations (e.g., clearing events, checking if an event is empty) are performed through public methods of the `EventManager`.
 * 
 * Note:
 * - The `EventManager` relies on a map (`mEventMap`) to store events and their associated delegates.
 * - Thread safety is not explicitly addressed; care must be taken when using this class in multithreaded environments.
 */
using EventManager = Delegate::Details::EventManager;