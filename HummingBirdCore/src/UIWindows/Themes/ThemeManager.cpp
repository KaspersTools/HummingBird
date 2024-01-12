//
// Created by Kasper de Bruin on 01/11/2023.
//

#include "ThemeManager.h"

namespace HummingBirdCore {
  namespace Themes {
    int ThemeManager::currentThemeIDX = 0;

    void ThemeManager::render() {
      ImGui::Text("Theme");

      std::vector<const char*> all_themes_names_antic;
      for (const auto &v: ALL_THEMES_NAMES)
        all_themes_names_antic.push_back(v.c_str());

      bool changed = ImGui::ListBox(
              "##Theme", &ThemeManager::currentThemeIDX, all_themes_names_antic.data(), all_themes_names_antic.size(), all_themes_names_antic.size());
      if (changed)
        ApplyTheme(ALL_THEMES[ThemeManager::currentThemeIDX]);
    }
  }// namespace Themes
}// namespace HummingBirdCore