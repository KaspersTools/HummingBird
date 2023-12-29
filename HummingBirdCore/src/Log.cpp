//
// Created by Kasper de Bruin on 02/11/2023.
//

#include "Log.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "Logging/ImGuiLogSink.h"
#include "Logging/MainLogSink.h"

namespace HummingBirdCore {
  Ref<spdlog::logger> Log::s_CoreLogger;
  bool Log::m_isInitialized = false;
  bool sinkToFile = false;
  std::vector<spdlog::sink_ptr> Log::s_logSinks = {};

  void Log::Init(){
    if (m_isInitialized) {
      CORE_ERROR("Log already initialized, not initializing again");
      return;
    }

    m_isInitialized = true;

    auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    consoleSink->set_pattern("%^[%T] %n: %v%$");
    s_logSinks.emplace_back(consoleSink);

    if (sinkToFile) {
      auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("HummingBirdCore.log", true);
      fileSink->set_pattern("[%T] [%l] %n: %v");
      s_logSinks.emplace_back(fileSink);
    }

    std::shared_ptr <HummingBirdCore::Logging::MainLogSink_mt> mainlogSink = std::make_shared<HummingBirdCore::Logging::MainLogSink_mt >();
    mainlogSink->set_pattern("[%T] [%l] %n: %v");
    s_logSinks.emplace_back(mainlogSink);

    s_CoreLogger = std::make_shared<spdlog::logger>("HummingBirdCore", begin(s_logSinks), end(s_logSinks));
    s_CoreLogger->set_level(spdlog::level::trace);
    spdlog::register_logger(s_CoreLogger);

    CORE_TRACE("Log initialized");
  }

}// namespace HummingBirdCore
