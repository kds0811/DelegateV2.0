#pragma once
#include <cstdint>
#include <functional>
#include <unordered_map>


namespace Delegate
{
  namespace Details
  {
    class BaseDelegate
    {
    public:
      BaseDelegate()          = default;
      virtual ~BaseDelegate() = default;
    };

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
