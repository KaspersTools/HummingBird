//
// Created by Kasper de Bruin on 01/11/2023.
//

#pragma once
#include "../UIWindow.h"
#include "Themes.h"

namespace HummingBirdCore {
  namespace Themes {
    enum THEMES {
      MAYA,
      PHOCOSGREEN,
      DARK,
      LIGHT,
      CLASSIC,
      MONOCHROME,
      THE_0N3,
      MODERNDARK,
      EMBRACETHEDARKNESS,
      DOUGHBKINS_BLACK,
      DOUGHBKINS_WHITE,
      BLENDER,
      Cyberpunk_T,
      Nord_T,
      THEMES_COUNT
    };

    class ThemeManager : public UIWindow {
  public:
      explicit ThemeManager(const std::string &name) : UIWindow(ImGuiWindowFlags_MenuBar, name) {}
      ~ThemeManager() = default;
      void render() override;
      void SelectThemeWindow();

      inline static void SetTheme() {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImGui::StyleColorsDark(style);//Reset to base/dark theme

        SetWindowBGAlpha(156);

        //Example of setting a theme
        Themes::Blender();
      }
    };
  }// namespace Themes
}// namespace HummingBirdCore
