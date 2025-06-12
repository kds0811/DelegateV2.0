#pragma once
#include <cstdint>
#include <functional>
#include <unordered_map>


namespace Delegate
{
  namespace Details
  {
    /**
    * The `BaseDelegate` class serves as an abstract base class for all delegate types.
    * It provides a common interface for managing event callbacks in the event-driven system.
    * 
    * Key Responsibilities:
    * - Acts as a polymorphic base class for derived `Delegate` templates.
    * - Ensures that derived classes can be stored in a unified container (e.g., `std::unordered_map`) within the `EventManager`.
    */
    class BaseDelegate
    {
    public:
      BaseDelegate()          = default;
      virtual ~BaseDelegate() = default;
    };


    /**
    * The `Delegate` class is a templated implementation responsible for managing event callbacks with specific argument types.
    * 
    * Key Responsibilities:
    * - Stores and manages a collection of callback functions (free functions or member functions) for a specific event.
    * - Provides methods to attach, detach, invoke, and clear callbacks.
    * 
    * Usage:
    * - Callbacks are identified by unique IDs and stored in an internal map (`mCallBackMap`).
    * - Free functions or member functions can be attached using the `Attach()` method, which returns a unique ID for later reference.
    * - All attached callbacks can be invoked using the `InvokeAll()` method with the required arguments.
    * 
    * Important Notes:
    * - Direct creation or management of `Delegate` instances is prohibited. 
    * - The `EventManager` is responsible for creating, storing, and managing `Delegate` instances, ensuring proper lifecycle management.
    * - The class supports move semantics but is non-copyable to prevent misuse.
    */
    template <typename... CallBackArgs>
    class Delegate : public BaseDelegate
    {
    public:
      using CallBackFunction = std::function<void(CallBackArgs...)>;
      using CallBackMap      = std::unordered_map<std::int32_t, CallBackFunction>;

    private:
      std::int32_t mID = 0;
      CallBackMap mCallBackMap{};


    public:
      Delegate() = default;

      Delegate(const Delegate&)            = delete;
      Delegate& operator=(const Delegate&) = delete;

      Delegate(Delegate&& other) noexcept : mID(other.mID), mCallBackMap(std::move(other.mCallBackMap)) {}

      Delegate& operator=(Delegate&& other) noexcept
      {
        if (this != &other)
        {
          mID          = other.mID;
          mCallBackMap = std::move(other.mCallBackMap);
        }
        return *this;
      }

      [[nodiscard]] inline std::int32_t Attach(CallBackFunction func)
      {
        auto id = GetID();
        mCallBackMap.emplace(id, func);
        return id;
      }

      template <typename T>
      [[nodiscard]] inline std::int32_t Attach(T* obj, void (T::*method)(CallBackArgs...))
      {
        auto id      = GetID();
        auto closure = [obj, method](CallBackArgs... args) { (obj->*method)(std::forward<CallBackArgs>(args)...); };
        mCallBackMap.emplace(id, std::move(closure));
        return id;
      }

      inline void Detach(std::int32_t id) { mCallBackMap.erase(id); }

      inline void InvokeAll(const CallBackArgs&... args) const
      {
        for (const auto& [id, func] : mCallBackMap)
        {
          std::invoke(func, args...);
        }
      }

      inline void Clear() noexcept { mCallBackMap.clear(); }

      [[nodiscard]] inline bool IsEmpty() const noexcept { return mCallBackMap.empty(); }

    private:
      [[nodiscard]] constexpr std::int32_t GetID() noexcept { return mID++; }
    };

  } // namespace Details

} // namespace Delegate
