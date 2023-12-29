//
// Created by Kasper de Bruin on 22/12/2023.
//
#pragma once
#include "Log.h"
#include "Logging/MainLogSink.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "UIWindow.h"

namespace HummingBirdCore::UIWindows {

  class LogWindow : public UIWindow {
public:
    LogWindow() : UIWindow(ImGuiWindowFlags_None, "Debug Window") {
    }

    ~LogWindow() = default;

    void render() override {
      std::vector<HummingBirdCore::Logging::ImGuiLogSinkItem> logMessages =
              HummingBirdCore::Logging::MainLogSink<std::mutex>::getInstance()->getLogMessages();

      for (auto &log: logMessages) {
        switch (log.level) {
//        TODO:
//          / Users / k.debruin / _private / HummingBird / HummingBirdCore / src / UIWindows / LogWindow.h : 46 : 64 : warning : format string is not a string literal(potentially insecure)[-Wformat - security] ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), log.message.c_str());
//
//          ^~~~~~~~~~~~~~~~~~~ / Users / k.debruin / _private / HummingBird / HummingBirdCore / src / UIWindows / LogWindow.h : 46 : 64 : note : treat the string as an argument to avoid this ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), log.message.c_str());
          case spdlog::level::info:

            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), log.message.c_str());
            break;
          case spdlog::level::warn:
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), log.message.c_str());
            break;
          case spdlog::level::err:
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), log.message.c_str());
            break;
          case spdlog::level::debug:
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), log.message.c_str());
            break;
          case spdlog::level::critical:
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), log.message.c_str());
            break;
          case spdlog::level::trace:
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), log.message.c_str());
            break;
          default:
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), log.message.c_str());
            break;
        }
      }
    }
  };
}// namespace HummingBirdCore::UIWindows