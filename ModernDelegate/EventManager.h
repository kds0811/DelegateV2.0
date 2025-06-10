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

      using EventMap = std::unordered_map<std::string, std::unique_ptr<BaseDelegate>>;

      EventMap mEventMap; // storage

    public:
      ~EventManager() {}
      [[nodiscard]] static EventManager* GetEventManager();

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
      EventManager(const EventManager&)                      = delete;
      EventManager(const EventManager&&) noexcept            = delete;
      EventManager& operator=(const EventManager&)           = delete;
      EventManager& operator=(const EventManager&&) noexcept = delete;


      template <typename... CallbackArgs>
      [[nodiscard]] std::optional<std::int32_t> AttachToEvent(
          std::string_view eventName,
          void (*func)(CallbackArgs...));

      template <typename T, typename... CallbackArgs>
      [[nodiscard]] std::optional<std::int32_t> AttachToEvent(
          std::string_view eventName,
          T* obj,
          void (T::*method)(CallbackArgs...));

      template <typename... CallbackArgs>
      void DetachFromEvent(std::string_view eventName, std::int32_t callBackID);

      [[nodiscard]] inline bool HasEvent(std::string_view eventName);

      template <typename... CallbackArgs>
      [[nodiscard]] Delegate<CallbackArgs...>* GetDelegate(std::string_view eventName);
    };

    EventManager* EventManager::GetEventManager()
    {
      static EventManager* instance = new EventManager();
      return instance;
    }

    template <typename... CallbackArgs>
    inline void EventManager::CreateEvent(std::string_view eventName)
    {
      if (!HasEvent(eventName))
      {
        auto dg = std::make_unique<Delegate::Delegate<CallbackArgs...>>();
        mEventMap.emplace(eventName, std::move(dg));
      }
      else
      {
        // LOG_ERROR("Event with ", nameEvent, " allready is created");
      }
    }

    template <typename... CallbackArgs>
    inline std::optional<std::int32_t> EventManager::AttachToEvent(
        std::string_view eventName,
        void (*func)(CallbackArgs...))
    {
      auto delegate = GetDelegate<CallbackArgs...>(eventName);
      if (delegate)
      {
        return delegate->Attach(func);
      }
      return std::nullopt;
    }

    template <typename T, typename... CallbackArgs>
    inline std::optional<std::int32_t> EventManager::AttachToEvent(
        std::string_view eventName,
        T* obj,
        void (T::*method)(CallbackArgs...))
    {
      auto delegate = GetDelegate<CallbackArgs...>(eventName);
      if (delegate)
      {
        return delegate->Attach(obj, method);
      }
      return std::nullopt;
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
      auto delegate = GetDelegate<CallbackArgs...>(nameEvent.data());
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
      if (!mEventMap.contains(nameEvent.data()))
      {
        // LOG_ERROR("An event named  ", nameEvent, "  doesn't exist");
        return false;
      }
      return true;
    }

  } // namespace Details

} // namespace Delegate
