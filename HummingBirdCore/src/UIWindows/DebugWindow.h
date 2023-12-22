//
// Created by Kasper de Bruin on 22/12/2023.
//
#pragma once
#include "UIWindow.h"

namespace HummingBirdCore::UIWindows {

  enum DebugLevel {
    DebugLevel_Info,
    DebugLevel_Warning,
    DebugLevel_Error
  };


  class DebugWindow : public UIWindow {
  public:
    DebugWindow() : UIWindow(ImGuiWindowFlags_None, "Debug Window"){

    }

    ~DebugWindow() = default;

    void render() override{

    }

private:

  };
} // namespace HummingBirdCore::UIWindows