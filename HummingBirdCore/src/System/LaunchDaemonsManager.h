//
// Created by Kasper de Bruin on 08/01/2024.
//

#pragma once

#include "UIWindows/Widget/DataViewer.h"
#include <PCH/pch.h>

namespace HummingBirdCore {
  namespace System {
    class LaunchDaemonsManager : public UIWindow {
  public:
      explicit LaunchDaemonsManager(const std::string &name) : UIWindow(ImGuiWindowFlags_None, name) {
        fetchAllDaemons();
      }

      ~LaunchDaemonsManager() = default;
      void render() override;

      void fetchAllDaemons();

  private:
      bool initialized = false;
    };

  }// namespace System
}// namespace HummingBirdCore
