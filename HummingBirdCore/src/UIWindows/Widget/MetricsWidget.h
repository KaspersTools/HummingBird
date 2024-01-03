//
// Created by Kasper de Bruin on 03/01/2024.
//

#pragma once
#include <PCH/pch.h>

#include "ThirdParty/MetricsImGui.h"
#include "UIWindows/UIWindow.h"

namespace HummingBirdCore::Widgets {
  class MetricsWidget : public UIWindow {
public:
        explicit MetricsWidget(const std::string &name) : UIWindow(ImGuiWindowFlags_MenuBar, name) {

        }

        ~MetricsWidget() = default;
        void render() override {
        }
  };
}

