//
// Created by Kasper de Bruin on 01/11/2023.
//

#include "ThemeManager.h"

namespace HummingBirdCore {
  namespace Themes {
    void ThemeManager::render() {
      SelectThemeWindow();
      ImGui::ShowStyleEditor();
    }

    void ThemeManager::SelectThemeWindow() {
      ImGui::Text("Select a theme:");
      ImGui::Separator();
    }
  }// namespace Themes
}// namespace HummingBirdCore