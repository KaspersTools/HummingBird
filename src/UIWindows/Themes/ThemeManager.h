//
// Created by Kasper de Bruin on 01/11/2023.
//

#ifndef KBTOOLS_THEMEMANAGER_H
#define KBTOOLS_THEMEMANAGER_H
#include "Themes.h"
#include "../UIWindow.h"

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
            EMBRACETHEDARKNESS,
            DOUGHBKINS_BLACK,
            DOUGHBKINS_WHITE,
        };
        class ThemeManager  : public UIWindow {
        public:
            ThemeManager() = default;
            ~ThemeManager() = default;
            void Render() override;
            void SelectThemeWindow();

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
                    case THEMES::DOUGHBKINS_BLACK:
                        DoughBkins_Black();
                        break;
                    case THEMES::DOUGHBKINS_WHITE:
                        DoughBkins_White();
                        break;
                }

                SetWindowBGAlpha(156);
            }

        };
    } // Themes
} // KBTools

#endif //KBTOOLS_THEMEMANAGER_H
