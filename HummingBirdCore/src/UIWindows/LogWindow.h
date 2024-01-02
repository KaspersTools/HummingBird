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
    LogWindow(const std::string& name) : UIWindow(ImGuiWindowFlags_None, name) {
    }

    ~LogWindow() = default;

    void render() override {
      std::vector<HummingBirdCore::Logging::ImGuiLogSinkItem> logMessages =
              HummingBirdCore::Logging::MainLogSink<std::mutex>::getInstance()->getLogMessages();

      for (auto &log: logMessages) {
        ImColor color = HummingBirdCore::Logging::getLogColor(log.level);
        ImGui::TextColored(color, log.message.c_str());
      }
    }
  };
}// namespace HummingBirdCore::UIWindows