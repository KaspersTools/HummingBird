//
// Created by Kasper de Bruin on 02/11/2023.
//

#pragma once

#include <memory>
#include <vector>

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

#include "CoreRef.h"

//Thanks @TheCherno
namespace HummingBirdCore {
    class Log
    {
    public:
        static void Init();

        static HummingBirdCore::Ref<spdlog::logger>& GetCoreLogger() {
          return s_CoreLogger;
        }

    private:
        static HummingBirdCore::Ref<spdlog::logger> s_CoreLogger;
        static std::vector<spdlog::sink_ptr> s_logSinks;
        static bool m_isInitialized;
        static bool m_syncToFile;
    };
}
// Core log macros
#define CORE_TRACE(...)    ::HummingBirdCore::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CORE_INFO(...)     ::HummingBirdCore::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CORE_WARN(...)     ::HummingBirdCore::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CORE_TRACE(...)    ::HummingBirdCore::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CORE_ERROR(...)    ::HummingBirdCore::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CORE_CRITICAL(...) ::HummingBirdCore::Log::GetCoreLogger()->critical(__VA_ARGS__)
