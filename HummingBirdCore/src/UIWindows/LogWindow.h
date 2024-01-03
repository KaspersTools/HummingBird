//
// Created by Kasper de Bruin on 22/12/2023.
//
#pragma once
#include <PCH/pch.h>
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
        std::string logMessage = log.message;
//        ImGui::TextColored(color, logMessage);
      }
    }
  };
}// namespace HummingBirdCore::UIWindows