//
// Created by Kasper de Bruin on 02/11/2023.
//

#include "Log.h"
#include <PCH/pch.h>

#include <spdlog/details/log_msg.h>
#include <spdlog/pattern_formatter.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace HummingBirdCore {
  std::shared_ptr<spdlog::logger> Log::s_coreLogger = nullptr;

  void Log::Init() {
    if (s_isInitialized)
      return;

    auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    consoleSink->set_pattern("%^[%T] %n | %s-%!():%# | %v%$");
    s_logSinks.emplace_back(consoleSink);

    auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("HummingBirdCore.log", true);
    fileSink->set_pattern("[source %s] [function %!] [line %#] %v");
    s_logSinks.emplace_back(fileSink);

    s_coreLogger = std::make_shared<spdlog::logger>("HummingBirdCore", begin(s_logSinks), end(s_logSinks));
    s_coreLogger->set_level(spdlog::level::trace);

    spdlog::register_logger(s_coreLogger);

    CORE_TRACE("Log initialized");
  }

  void Log::log(spdlog::source_loc loc, spdlog::level::level_enum lvl, spdlog::string_view_t msg) {
    getCoreLogger()->log(loc, lvl, msg);

    for (auto sink: getCoreLogger()->sinks())
      if (sink->should_log(lvl)) {
        notify(loc, lvl, msg);
        return;
      }
  }

  void Log::notify(spdlog::source_loc loc, spdlog::level::level_enum lvl, spdlog::string_view_t msg) {

    if(lvl == spdlog::level::off || lvl == spdlog::level::trace)
      return;

    spdlog::details::log_msg logMsg(loc, "notify", lvl, msg);

    std::unique_ptr<spdlog::formatter> formatter;

    std::string patternForTitle = "%^[%T] %n";
    formatter = std::unique_ptr<spdlog::formatter>(new spdlog::pattern_formatter(patternForTitle));
    fmt::basic_memory_buffer<char, 250> formatted;
    formatter->format(logMsg, formatted);
    std::string formattedTitleStr = std::string(formatted.data(), formatted.size());

    std::string patternForMessage = "%v";
    formatted = fmt::basic_memory_buffer<char, 250>();
    formatter = std::unique_ptr<spdlog::formatter>(new spdlog::pattern_formatter(patternForMessage));
    formatter->format(logMsg, formatted);
    std::string formattedData = std::string(formatted.data(), formatted.size());

    auto type = ImGuiToastType_None;

    switch (lvl) {
      case spdlog::level::trace:
        type = ImGuiToastType_Info;
        break;
      case spdlog::level::debug:
        type = ImGuiToastType_Info;
        break;
      case spdlog::level::info:
        type = ImGuiToastType_Info;
        break;
      case spdlog::level::warn:
        type = ImGuiToastType_Warning;
        break;
      case spdlog::level::err:
        type = ImGuiToastType_Error;
        break;
      case spdlog::level::critical:
        type = ImGuiToastType_Error;
        break;
      case spdlog::level::off:
        type = ImGuiToastType_None;
        break;
    }

    ImGuiToast toast = ImGuiToast(type, formattedTitleStr, formattedData, 5000);
    ImGui::InsertNotification(toast);
  }

}// namespace HummingBirdCore
