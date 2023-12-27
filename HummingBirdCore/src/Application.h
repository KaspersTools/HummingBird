//
//
// Created by Kasper de Bruin on 01/11/2023.
//

#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <string>

#include <iostream>
#include <vector>


// Dear ImGui
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl2.h>
#include <imgui.h>

// SDL
#include <SDL.h>
#include <glad/glad.h>

// spdlog
#include <spdlog/spdlog.h>

// UIWindows
#include "UIWindows/ContentExplorer.h"
#include "UIWindows/LogWindow.h"
#include "UIWindows/UIWindow.h"

#include "Terminal/TerminalWindow.h"

#include "Security/LogInWindow.h"
#include "Security/LoginManager.h"

#include "System/Hosts/EditHostsWindow.h"

#include "UIWindows/LogWindow.h"
#include "UIWindows/Themes/ThemeManager.h"

// Rendering
#include "Rendering/Texture.h"

// Sql
#include "Sql/SqlManager.h"

// Utils
#include "Utils/Input.h"

// Kasper specific
#ifdef WITHHUMMINGBIRDKASPERSPECIFIC
  #include <HelloLibrary.h>
  #include <Hellebrekers/VisualLinkLauncher.h>

  namespace HummingBirdKasper::VisualLink {
    class VisualLinkLauncher;
  }
#endif

namespace HummingBirdCore {
  class Application {
public:
    Application();

    ~Application();

    //TODO: find more intuitive way to add windows
    UIWindow &AddWindow(const std::string &name, std::shared_ptr<UIWindow> uiWindow) {
      //check if is instance of UIWindow
      std::string finalName = "";
      if (std::dynamic_pointer_cast<Terminal::TerminalWindow>(uiWindow)) {
        m_terminalCount++;
        finalName = name + std::to_string(m_terminalCount);
      } else if (std::dynamic_pointer_cast<System::EditHostsWindow>(uiWindow)) {
        m_editHostsCount++;
        finalName = name + std::to_string(m_editHostsCount);
      } else if (std::dynamic_pointer_cast<Themes::ThemeManager>(uiWindow)) {
        m_themeManagerCount++;
        finalName = name + std::to_string(m_themeManagerCount);
      } else if (std::dynamic_pointer_cast<Security::LogInWindow>(uiWindow)) {
        finalName = name;
      } else if(std::dynamic_pointer_cast<UIWindows::LogWindow>(uiWindow)) {
        m_logWindowCount++;
        finalName = name + std::to_string(m_logWindowCount);
      }
      //Kasper specific
#ifdef WITHHUMMINGBIRDKASPERSPECIFIC
      else if (std::dynamic_pointer_cast<HummingBirdKasper::VisualLink::VisualLinkLauncher>(uiWindow)) {
        m_visualLinkLauncherCount++;
        finalName = name + std::to_string(m_visualLinkLauncherCount);
      }
#endif
      else {
        CORE_WARN("UIWindow {0} is not a valid type", name);
        finalName = name;
      }

      m_uiWindows[finalName] = uiWindow;
      return *uiWindow;
    }


private:
    void InitSDL();

    void InitImGui();

    void Run();

    void RenderUI();

    void SetupDockspace();

    void Render();

public:
    const int GetWindowWidth() const { return m_windowWidth; }
    const int GetWindowHeight() const { return m_windowHeight; }

    static SDL_Window *GetWindow() { return s_window; }
    static Application *GetApplication() {
      return s_application;
    }

private:
    inline static SDL_Window *s_window;
    inline static Application *s_application = nullptr;

    SDL_GLContext m_gl_context{};
    bool m_exit = false;
    GLsizei m_windowWidth = 1920;
    GLsizei m_windowHeight = 1080;
    Texture m_backgroundTexture = Texture("Assets/Textures/newbg.png");

    // UIWindows
    std::map<std::string, std::shared_ptr<UIWindow>> m_uiWindows;
    Security::LogInWindow m_loginWindow;

    //imgui third party windows
    bool m_showDemoWindow = false;
    bool m_showMetricsWindow = false;

    int m_terminalCount = 0;
    int m_editHostsCount = 0;
    int m_themeManagerCount = 0;
    int m_logWindowCount = 0;

#ifdef WITHHUMMINGBIRDKASPERSPECIFIC
    int m_visualLinkLauncherCount = 0;
#endif

    //    bool m_ShowExampleAppLongText = false;
    //    bool m_ShowExampleAppAutoResize = false;
    //
  };

}// namespace HummingBirdCore