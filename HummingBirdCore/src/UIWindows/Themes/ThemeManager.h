//
// Created by Kasper de Bruin on 01/11/2023.
//

#pragma once
#include "../UIWindow.h"
#include <UIWindows/Themes/Themes.h>

namespace HummingBirdCore::Themes{
  const std::vector<ImGuiTheme_> ALL_THEMES = {
          ImGuiTheme_ImGuiColorsClassic,
          ImGuiTheme_ImGuiColorsDark,
          ImGuiTheme_ImGuiColorsLight,
          ImGuiTheme_MaterialFlat,
          ImGuiTheme_PhotoshopStyle,
          ImGuiTheme_GrayVariations,
          ImGuiTheme_GrayVariations_Darker,
          ImGuiTheme_MicrosoftStyle,
          ImGuiTheme_Cherry,
          ImGuiTheme_Darcula,
          ImGuiTheme_DarculaDarker,
          ImGuiTheme_LightRounded,
          ImGuiTheme_SoDark_AccentBlue,
          ImGuiTheme_SoDark_AccentYellow,
          ImGuiTheme_SoDark_AccentRed,
          ImGuiTheme_BlackIsBlack,
          ImGuiTheme_WhiteIsWhite,
  };

  const std::vector<std::string> ALL_THEMES_NAMES = {
          "ImGuiColorsClassic",
          "ImGuiColorsDark",
          "ImGuiColorsLight",
          "MaterialFlat",
          "PhotoshopStyle",
          "GrayVariations",
          "GrayVariations_Darker",
          "MicrosoftStyle",
          "Cherry",
          "Darcula",
          "DarculaDarker",
          "LightRounded",
          "SoDark_AccentBlue",
          "SoDark_AccentYellow",
          "SoDark_AccentRed",
          "BlackIsBlack",
          "WhiteIsWhite"};
  class ThemeManager : public UIWindow {
public:
    ThemeManager(const std::string &name) : UIWindow(name, ImGuiWindowFlags_None) {

    }

    ~ThemeManager() = default;
public:
    void render() override;
private:
    static int currentThemeIDX;
  };
}// namespace HummingBirdCore::Themes
