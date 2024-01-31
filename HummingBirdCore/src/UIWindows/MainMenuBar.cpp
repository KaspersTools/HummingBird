//
// Created by Kasper de Bruin on 23/01/2024.
//
#include <UIWindows/MainMenuBar.h>
#include <KDB_ImGui/backends/imgui_impl_glfw_vulkan_window.h>

// UIWindows
#include "UIWindows/ContentExplorer.h"
#include "UIWindows/LogWindow.h"
#include "UIWindows/UIWindow.h"

// UIWindows/Themes
#include "UIWindows/Themes/ThemeManager.h"
#include "UIWindows/Themes/Themes.h"

// UIWindows/Widget
#include "UIWindows/Widget/AllAddonsWidget.h"
#include "UIWindows/Widget/DataViewer.h"
#include "UIWindows/Widget/MetricsWidget.h"

// OTHER WINDOWS
#include "Terminal/TerminalWindow.h"

#include "Security/LogInWindow.h"
#include "Security/LoginManager.h"

#include "System/Hosts/EditHostsWindow.h"
#include "System/LaunchDaemonsManager.h"
#include "System/SysInfoWindow.h"

// System
#include "System/SysInfo.h"
#include "WindowManager.h"

namespace HummingBirdCore {
  void UI::mainMenuBarCallback() {
    HummingBirdCore::UI::WindowManager *windowManager = HummingBirdCore::UI::WindowManager::getInstance();

    // File menu
    if (ImGui::BeginMenu("File")) {
      // Project management
      if (ImGui::MenuItem("New Project")) {
        CORE_WARN("NEW PROJECT NOT IMPLEMENTED");
      }
      if (ImGui::MenuItem("Open Project")) {
        CORE_WARN("OPEN PROJECT NOT IMPLEMENTED");
      }
      if (ImGui::MenuItem("Save Project")) {
        CORE_WARN("SAVE PROJECT NOT IMPLEMENTED");
      }

      ImGui::Separator();

      // Session management
      if (ImGui::MenuItem("Log Out")) {
        CORE_WARN("LOG OUT NOT IMPLEMENTED");
      }
      if (ImGui::MenuItem("Exit")) {
        ImGui_ImplVKGlfw_setShouldClose(true);
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("SQL")) {
      ImGui::BeginDisabled(true);
      if (ImGui::MenuItem("Sql Connect")) {
        const std::string baseName = "Sql ";
//        windowManager->addWindow(baseName, 0, std::make_shared<HummingBirdCore::Sql::SqlWindow>(baseName));
      }
      if (ImGui::MenuItem("Import SQL")) {
        const std::string baseName = "Import SQL ";
//        windowManager->addWindow(baseName, 0, std::make_shared<HummingBirdCore::Sql::SqlImportWindow>(baseName));
      }
      ImGui::EndDisabled();
      ImGui::EndMenu();
    }

    // Tools menu
    if (ImGui::BeginMenu("Tools")) {
      // System Tools
      if (ImGui::BeginMenu("System Tools")) {
        if (ImGui::MenuItem("System Info")) {
          const std::string baseName = "System Info ";
          windowManager->addWindow(baseName, 0, std::make_shared<HummingBirdCore::System::SysInfoWindow>(baseName));
        }
        if (ImGui::MenuItem("Edit Hosts")) {
          const std::string baseName = "Edit Hosts ";
          windowManager->addWindow(baseName, 0, std::make_shared<HummingBirdCore::System::EditHostsWindow>(baseName));
        }

        if (ImGui::MenuItem("Launch daemons")) {
          const std::string baseName = "Launch daemons ";
          windowManager->addWindow(baseName, 0, std::make_shared<HummingBirdCore::System::LaunchDaemonsManager>(baseName));
        }

        ImGui::EndMenu();
      }
      ImGui::EndMenu();
    }

    // Developer Tools
    if (ImGui::BeginMenu("Developer Tools")) {
      if (ImGui::MenuItem("Terminal")) {
        const std::string baseName = "Terminal ";
        windowManager->addWindow(baseName, 0, std::make_shared<HummingBirdCore::Terminal::TerminalWindow>(baseName));
      }
      if (ImGui::MenuItem("Debug Window")) {
        const std::string baseName = "Debug Window ";
//        windowManager->addWindow(baseName, 0, std::make_shared<HummingBirdCore::UI::LogWindow>(baseName));
      }
      ImGui::EndMenu();
    }

    // Additional Tools
    if (ImGui::BeginMenu("Additional Tools")) {
      if (ImGui::MenuItem("Data Viewer")) {
        const std::string baseName = "Data Viewer ";
        windowManager->addWindow(baseName, 0, std::make_shared<HummingBirdCore::Widgets::DataViewer>(baseName));
      }

      if (ImGui::MenuItem("Content Explorer")) {
        const std::string baseName = "Content Explorer ";
        windowManager->addWindow(baseName, 0, std::make_shared<HummingBirdCore::UIWindows::ContentExplorer>(baseName));
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("View")) {
      if (ImGui::BeginMenu("Styles")) {
        if (ImGui::MenuItem("ThemeManager")) {
          const std::string baseName = "ThemeManager ";
          WindowManager().addWindow(baseName, 0, std::make_shared<HummingBirdCore::Themes::ThemeManager>(baseName));
        }
        if (ImGui::BeginMenu("Themes")) {
          if (ImGui::MenuItem("ImGuiColorsClassic")) {
            Themes::ApplyTheme(Themes::ImGuiTheme_ImGuiColorsClassic);
          }
          if (ImGui::MenuItem("ImGuiColorsDark")) {
            Themes::ApplyTheme(Themes::ImGuiTheme_ImGuiColorsDark);
          }
          if (ImGui::MenuItem("ImGuiColorsLight")) {
            Themes::ApplyTheme(Themes::ImGuiTheme_ImGuiColorsLight);
          }
          if (ImGui::MenuItem("MaterialFlat")) {
            Themes::ApplyTheme(Themes::ImGuiTheme_MaterialFlat);
          }
          if (ImGui::MenuItem("PhotoshopStyle")) {
            Themes::ApplyTheme(Themes::ImGuiTheme_PhotoshopStyle);
          }
          if (ImGui::MenuItem("GrayVariations")) {
            Themes::ApplyTheme(Themes::ImGuiTheme_GrayVariations);
          }
          if (ImGui::MenuItem("GrayVariations_Darker")) {
            Themes::ApplyTheme(Themes::ImGuiTheme_GrayVariations_Darker);
          }
          if (ImGui::MenuItem("MicrosoftStyle")) {
            Themes::ApplyTheme(Themes::ImGuiTheme_MicrosoftStyle);
          }
          if (ImGui::MenuItem("Cherry")) {
            Themes::ApplyTheme(Themes::ImGuiTheme_Cherry);
          }
          if (ImGui::MenuItem("Darcula")) {
            Themes::ApplyTheme(Themes::ImGuiTheme_Darcula);
          }
          if (ImGui::MenuItem("DarculaDarker")) {
            Themes::ApplyTheme(Themes::ImGuiTheme_DarculaDarker);
          }
          if (ImGui::MenuItem("LightRounded")) {
            Themes::ApplyTheme(Themes::ImGuiTheme_LightRounded);
          }
          if (ImGui::MenuItem("SoDark_AccentBlue")) {
            Themes::ApplyTheme(Themes::ImGuiTheme_SoDark_AccentBlue);
          }
          if (ImGui::MenuItem("SoDark_AccentYellow")) {
            Themes::ApplyTheme(Themes::ImGuiTheme_SoDark_AccentYellow);
          }
          if (ImGui::MenuItem("SoDark_AccentRed")) {
            Themes::ApplyTheme(Themes::ImGuiTheme_SoDark_AccentRed);
          }
          if (ImGui::MenuItem("BlackIsBlack")) {
            Themes::ApplyTheme(Themes::ImGuiTheme_BlackIsBlack);
          }
          if (ImGui::MenuItem("WhiteIsWhite")) {
            Themes::ApplyTheme(Themes::ImGuiTheme_WhiteIsWhite);
          }
          ImGui::EndMenu();
        }
        ImGui::EndMenu();
      }
      ImGui::EndMenu();
    }
  }
}// namespace HummingBirdCore