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

      template <typename... CallbackArgs>
      explicit ScopedEventHandler(const std::string_view eventName, void (*func)(CallbackArgs...)) : mEventName(eventName)
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


      inline ~ScopedEventHandler()
      {
        if (IsInitialized())
        {
          Detach();
        }
      }

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

using ScopedEventHandler = Delegate::Details::ScopedEventHandler;