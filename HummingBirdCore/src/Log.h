//
// Created by Kasper de Bruin on 02/11/2023.
//

#pragma once

#include <memory>
#include <vector>

#pragma warning(push, 0)
#include <spdlog/common.h>
#include <spdlog/details/backtracer.h>
#include <spdlog/details/log_msg.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>
#pragma warning(pop)

#include "CoreRef.h"
#include <imgui_notify.h>

//Thanks @TheCherno
namespace HummingBirdCore {
  class Log {
public:
    static void Init();

    static HummingBirdCore::Ref<spdlog::logger> &GetCoreLogger() {
      return s_CoreLogger;
    }

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
    inline static void notify(spdlog::source_loc loc, spdlog::level::level_enum lvl, spdlog::string_view_t msg) {
      std::string titleStr = s_CoreLogger->name();
      std::string contentStr = std::string(msg.data(), msg.size());

      char title[titleStr.length() + 1];
      strcpy(title, titleStr.c_str());

      char content[contentStr.length() + 1];
      strcpy(content, contentStr.c_str());

      ImGuiToast toast = ImGuiToast(ImGuiToastType_None, 5000);
      bool show = true;
      switch (lvl) {
        case spdlog::level::trace:
          show = false;
          toast.set_type(ImGuiToastType_Info);
          break;
        case spdlog::level::debug:
          toast.set_type(ImGuiToastType_Info);
          break;
        case spdlog::level::info:
          toast.set_type(ImGuiToastType_Info);
          break;
        case spdlog::level::warn:
          toast.set_type(ImGuiToastType_Warning);
          break;
        case spdlog::level::err:
          toast.set_type(ImGuiToastType_Error);
          break;
        case spdlog::level::critical:
          toast.set_type(ImGuiToastType_Error);
          break;
        case spdlog::level::off:
          toast.set_type(ImGuiToastType_None);
          break;
        default:
          toast.set_type(ImGuiToastType_None);
          break;
      }

      if (show) {
        toast.set_title(title);
        toast.set_content(content);

        ImGui::InsertNotification(toast);
      }
    }

    inline static void log(spdlog::source_loc loc, spdlog::level::level_enum lvl, spdlog::string_view_t msg) {
      s_CoreLogger->log(loc, lvl, msg);

      for (auto sink: s_CoreLogger->sinks())
        if (sink->should_log(lvl)) {
          notify(loc, lvl, msg);
          return;
        }
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


private:
    static HummingBirdCore::Ref<spdlog::logger> s_CoreLogger;
    static std::vector<spdlog::sink_ptr> s_logSinks;
    static bool s_isInitialized;
    static bool s_sinkToFile;
  };
}// namespace HummingBirdCore
// Core log macros
#define CORE_TRACE(...) ::HummingBirdCore::Log::trace(__VA_ARGS__)
#define CORE_INFO(...) ::HummingBirdCore::Log::info(__VA_ARGS__)
#define CORE_WARN(...) ::HummingBirdCore::Log::warn(__VA_ARGS__)
#define CORE_ERROR(...) ::HummingBirdCore::Log::error(__VA_ARGS__)
#define CORE_CRITICAL(...) ::HummingBirdCore::Log::critical(__VA_ARGS__)
