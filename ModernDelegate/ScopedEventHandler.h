#pragma once
#include "EventManager.h"
#include <optional>


namespace Delegate
{
  namespace Details
  {

    class ScopedEventHandler
    {
      std::optional<std::int32_t> mCallBackID{};
      std::string mEventName{};

    public:
      ScopedEventHandler() {}
      ScopedEventHandler(const ScopedEventHandler&)                = delete;
      ScopedEventHandler(ScopedEventHandler&&) noexcept            = delete;
      ScopedEventHandler& operator=(const ScopedEventHandler&)     = delete;
      ScopedEventHandler& operator=(ScopedEventHandler&&) noexcept = delete;
      ~ScopedEventHandler()
      {
        if (IsInitialized())
        {
          Detach();
        }
      }

      template <typename... CallbackArgs>
      explicit ScopedEventHandler(const std::string_view eventName, void (*func)(CallbackArgs...))
        : mEventName(eventName)
      {
        AttachImpl(mEventName, func);
      }

      template <typename T, typename... CallbackArgs>
      explicit ScopedEventHandler(const std::string_view eventName, T* obj, void (T::*method)(CallbackArgs...))
        : mEventName(eventName)
      {
        AttachImpl(mEventName, obj, method);
      }

      template <typename... CallbackArgs>
      void Attach(const std::string_view eventName, void (*func)(CallbackArgs...))
      {
        if (IsInitialized())
          Detach();

        mEventName = eventName;
        AttachImpl(mEventName, func);
      }

      template <typename T, typename... CallbackArgs>
      void Attach(const std::string_view eventName, T* obj, void (T::*method)(CallbackArgs...))
      {
        if (IsInitialized())
          Detach();

        mEventName = eventName;
        AttachImpl(mEventName, obj, method);
      }

      inline void Detach()
      {
        if (!IsInitialized())
          return;

        auto eventManager = EventManager::Get();
        if (eventManager)
        {
          eventManager->DetachFromEvent(mEventName, mCallBackID.value());
          mEventName  = "";
          mCallBackID = std::nullopt;
        }
      }

      [[nodiscard]] inline bool IsInitialized() const { return mCallBackID.has_value(); }


    private:
      template <typename... CallbackArgs>
      void AttachImpl(const std::string_view eventName, CallbackArgs&&... args)
      {
        auto eventManager = EventManager::Get();
        if (eventManager)
        {
          mCallBackID = eventManager->AttachToEvent(eventName, std::forward<CallbackArgs>(args)...);
        }
      }
    };

  } // namespace Details

} // namespace Delegate

/**
 * The `ScopedEventHandler` class is responsible for managing event subscriptions in an event-driven system.
 * 
 * Key Responsibilities:
 * - Attaches free functions or member functions to named events via the `EventManager`.
 * - Ensures automatic detachment of the event handler when the object goes out of scope, preventing resource leaks.
 * - Uses RAII principles to manage the lifecycle of event subscriptions.
 * 
 * Usage:
 * - Provides a simple interface for attaching and detaching event handlers.
 * - Automatically detaches the handler in the destructor, ensuring proper cleanup without manual intervention.
 */
using ScopedEventHandler = Delegate::Details::ScopedEventHandler;