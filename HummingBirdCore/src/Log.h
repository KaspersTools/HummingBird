//
// Created by Kasper de Bruin on 02/11/2023.
//

#pragma once

#include <memory>
#include <vector>

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

//Thanks @TheCherno
namespace HummingBirdCore {
    template<typename T>
    using Scope = std::unique_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Scope<T> CreateScope(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    class Log
    {
    public:
        static void Init();

        static Ref<spdlog::logger>& GetCoreLogger() {
          return s_CoreLogger;
        }

    private:
        static Ref<spdlog::logger> s_CoreLogger;
        static bool m_isInitialized;
        static bool m_syncToFile;
        static std::vector<spdlog::sink_ptr> s_logSinks;
    };
}
// Core log macros
#define CORE_TRACE(...)    ::HummingBirdCore::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CORE_INFO(...)     ::HummingBirdCore::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CORE_WARN(...)     ::HummingBirdCore::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CORE_TRACE(...)    ::HummingBirdCore::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CORE_ERROR(...)    ::HummingBirdCore::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CORE_CRITICAL(...) ::HummingBirdCore::Log::GetCoreLogger()->critical(__VA_ARGS__)
