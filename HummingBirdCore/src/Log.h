//
// Created by Kasper de Bruin on 02/11/2023.
//

#pragma once
#include <memory>

#include <spdlog/spdlog.h>

#include "CoreRef.h"

//Thanks @TheCherno
namespace HummingBirdCore {
  class Log {
public:
    static void Init();
    inline static void notify(spdlog::source_loc loc, spdlog::level::level_enum lvl, spdlog::string_view_t msg);
    inline static void log   (spdlog::source_loc loc, spdlog::level::level_enum lvl, spdlog::string_view_t msg);

    template<typename... Args>
    inline static void log(spdlog::source_loc loc, spdlog::level::level_enum lvl, spdlog::format_string_t<Args...> fmt, Args &&...args) {
      log(loc, lvl, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    inline static void log(spdlog::level::level_enum lvl, spdlog::format_string_t<Args...> fmt, Args &&...args) {
      log(spdlog::source_loc{}, lvl, fmt, std::forward<Args>(args)...);
    }

    template<typename T>
    inline static void log(spdlog::level::level_enum lvl, const T &msg) {
      log(spdlog::source_loc{}, lvl, msg);
    }

    template<typename... Args>
    inline static void trace(spdlog::format_string_t<Args...> fmt, Args &&...args) {
      log(spdlog::level::trace, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    inline static void debug(spdlog::format_string_t<Args...> fmt, Args &&...args) {
      log(spdlog::level::debug, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    inline static void info(spdlog::format_string_t<Args...> fmt, Args &&...args) {
      log(spdlog::level::info, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    inline static void warn(spdlog::format_string_t<Args...> fmt, Args &&...args) {
      log(spdlog::level::warn, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    inline static void error(spdlog::format_string_t<Args...> fmt, Args &&...args) {
      log(spdlog::level::err, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    inline static void critical(spdlog::format_string_t<Args...> fmt, Args &&...args) {
      log(spdlog::level::critical, fmt, std::forward<Args>(args)...);
    }


    template<typename T>
    inline static void trace(const T &msg) {
      log(spdlog::level::trace, msg);
    }

    template<typename T>
    inline static void debug(const T &msg) {
      log(spdlog::level::debug, msg);
    }

    template<typename T>
    inline static void info(const T &msg) {
      log(spdlog::level::info, msg);
    }

    template<typename T>
    inline static void warn(const T &msg) {
      log(spdlog::level::warn, msg);
    }

    template<typename T>
    inline static void error(const T &msg) {
      log(spdlog::level::err, msg);
    }

    template<typename T>
    inline static void critical(const T &msg) {
      log(spdlog::level::critical, msg);
    }

    static HummingBirdCore::Ref<spdlog::logger> &getCoreLogger() {
      return s_coreLogger;
    }

    inline static void getCoreLogLevel(spdlog::level::level_enum level) {
      getCoreLogger()->set_level(level);
    }

private:
    static HummingBirdCore::Ref<spdlog::logger> s_coreLogger;
    inline static std::vector<spdlog::sink_ptr> s_logSinks = {};
    inline static bool s_isInitialized = false;
  };
}// namespace HummingBirdCore

// Core log macros
#define CORE_TRACE(...) ::HummingBirdCore::Log::log(spdlog::source_loc(__FILE__, __LINE__, __FUNCTION__), \
                                                    spdlog::level::trace, __VA_ARGS__)
#define CORE_INFO(...) ::HummingBirdCore::Log::log(spdlog::source_loc(__FILE__, __LINE__, __FUNCTION__), \
                                                   spdlog::level::info, __VA_ARGS__)
#define CORE_WARN(...) ::HummingBirdCore::Log::log(spdlog::source_loc(__FILE__, __LINE__, __FUNCTION__), \
                                                   spdlog::level::warn, __VA_ARGS__)
#define CORE_ERROR(...) ::HummingBirdCore::Log::log(spdlog::source_loc(__FILE__, __LINE__, __FUNCTION__), \
                                                    spdlog::level::err, __VA_ARGS__)