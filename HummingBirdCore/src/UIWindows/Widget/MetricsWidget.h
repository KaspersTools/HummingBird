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
        explicit MetricsWidget(const std::string &name) : UIWindow(name, ImGuiWindowFlags_MenuBar) {

        }

        ~MetricsWidget() = default;
        void render() override {
        }
  };
}

