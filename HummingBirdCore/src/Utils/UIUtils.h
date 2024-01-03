//
// Created by Kasper de Bruin on 03/01/2024.
//

#pragma once
#include <imgui.h>

namespace ImGui {
  static float GetContentRegionAvailWidth() {
    return ImGui::GetContentRegionAvail().x;
  }
  static float GetContentRegionAvailHeight() {
    return ImGui::GetContentRegionAvail().y;
  }
}// namespace ImGui
