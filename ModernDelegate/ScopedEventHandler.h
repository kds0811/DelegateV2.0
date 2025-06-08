#pragma once
#include "EventManager.h"
#include <optional>


namespace Delegate
{
  class ScopedEventHandler
  {
    std::optional<std::int32_t> mCallBackID{};
    std::string mEventName{};

  public:
    template <typename... CallbackArgs>
    explicit ScopedEventHandler(const std::string& eventName, void (*func)(CallbackArgs...)) : mEventName(eventName)
    {
      Attach(mEventName, func);
    }

    template <typename T, typename... CallbackArgs>
    explicit ScopedEventHandler(const std::string& eventName, T* obj, void (T::*method)(CallbackArgs...))
      : mEventName(eventName)
    {
      Attach(mEventName, obj, method);
    }

    inline void Detach()
    {
      if (!IsInitialized())
        return;

      auto eventManager = EventManager::GetEventManager();
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
    void Attach(const std::string& eventName, CallbackArgs&&... args)
    {
      auto eventManager = EventManager::GetEventManager();
      if (eventManager)
      {
        mCallBackID = eventManager->AttachToEvent(eventName, std::forward<CallbackArgs>(args)...);
      }
    }
  };

} // namespace Delegate
