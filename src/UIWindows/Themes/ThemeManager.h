//
// Created by Kasper de Bruin on 01/11/2023.
//

#ifndef KBTOOLS_THEMEMANAGER_H
#define KBTOOLS_THEMEMANAGER_H
#include "Themes.h"

namespace KBTools {
    namespace Themes {
        enum class THEMES {
            MAYA,
            PHOCOSGREEN,
            DARK,
            LIGHT,
            CLASSIC,
            MONOCHROME,
            THE_0N3,
            MODERNDARK,
            EMBRACETHEDARKNESS
        };
        class ThemeManager {
        public:
            static void SetTheme(THEMES theme){
                switch (theme) {
                    case THEMES::MAYA:
                        Maya();
                        break;
                    case THEMES::PHOCOSGREEN:
                        PhocosGreen();
                        break;
                    case THEMES::DARK:
                        ImGui::StyleColorsDark();
                        break;
                    case THEMES::LIGHT:
                        ImGui::StyleColorsLight();
                        break;
                    case THEMES::CLASSIC:
                        ImGui::StyleColorsClassic();
                        break;
                    case THEMES::MONOCHROME:
                        Monochrome();
                        break;
                    case THEMES::THE_0N3:
                        The_0n3();
                        break;
                    case THEMES::MODERNDARK:
                        ModernDarkTheme();
                        break;
                    case THEMES::EMBRACETHEDARKNESS:
                        EmbraceTheDarkness();
                        break;
                };

                ImVec4 bgColor = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];

                //TODO: CHANGE CLEAR COLOR OF SDL
//                glClearColor(bgColor.x, bgColor.y, bgColor.z, bgColor.w);
            }
        };
    } // Themes
} // KBTools

#endif //KBTOOLS_THEMEMANAGER_H
