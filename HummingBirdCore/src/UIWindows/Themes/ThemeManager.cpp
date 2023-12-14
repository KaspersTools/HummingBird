//
// Created by Kasper de Bruin on 01/11/2023.
//

#include "ThemeManager.h"

namespace HummingBirdCore {
    namespace Themes{

        void ThemeManager::Render() {
            SelectThemeWindow();
            ImGui::ShowStyleEditor();
        }

        void ThemeManager::SelectThemeWindow() {
            ImGui::Text("Select a theme:");
            ImGui::Separator();
            if (ImGui::Button("Maya")) {
                SetTheme(THEMES::MAYA);
            }
            ImGui::SameLine();
            if (ImGui::Button("PhocosGreen")) {
                SetTheme(THEMES::PHOCOSGREEN);
            }
            ImGui::SameLine();
            if (ImGui::Button("Dark")) {
                SetTheme(THEMES::DARK);
            }
            ImGui::SameLine();
            if (ImGui::Button("Light")) {
                SetTheme(THEMES::LIGHT);
            }
            ImGui::SameLine();
            if (ImGui::Button("Classic")) {
                SetTheme(THEMES::CLASSIC);
            }
            ImGui::SameLine();
            if (ImGui::Button("Monochrome")) {
                SetTheme(THEMES::MONOCHROME);
            }
            ImGui::SameLine();
            if (ImGui::Button("The 0n3")) {
                SetTheme(THEMES::THE_0N3);
            }
            ImGui::SameLine();
            if (ImGui::Button("ModernDark")) {
                SetTheme(THEMES::MODERNDARK);
            }
            ImGui::SameLine();
            if (ImGui::Button("EmbraceTheDarkness")) {
                SetTheme(THEMES::EMBRACETHEDARKNESS);
            }
            ImGui::SameLine();
            if (ImGui::Button("DoughBkins_Black")) {
                SetTheme(THEMES::DOUGHBKINS_BLACK);
            }
            ImGui::SameLine();
            if (ImGui::Button("DoughBkins_White")) {
                SetTheme(THEMES::DOUGHBKINS_WHITE);
            }
        }
    } // Themes
} // KBTools