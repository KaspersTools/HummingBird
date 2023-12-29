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
#include "System/SysInfoWindow.h"

#include "UIWindows/LogWindow.h"
#include "UIWindows/Themes/ThemeManager.h"

#include "Sql/SqlWindow.h"

// Rendering
#include "Rendering/Texture.h"

// Utils
#include "Utils/Input.h"

// System
#include "System/SysInfo.h"


// Kasper specific
#ifdef WITHHUMMINGBIRDKASPERSPECIFIC
#include <Hellebrekers/VisualLinkLauncher.h>
#include <HelloLibrary.h>

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
      m_uiWindows[name] = uiWindow;
      return *uiWindow;
    }

    void AddUpdatable(std::shared_ptr<Updatable> updatable) {
      m_updatables.emplace_back(updatable);
    }


    //Getters
    const int GetWindowWidth() const { return m_windowWidth; }
    const int GetWindowHeight() const { return m_windowHeight; }

    static SDL_Window *GetWindow() { return s_window; }
    static Application *GetApplication() {
      return s_application;
    }

private:
    void InitSDL();

    void InitImGui();

    void Run();

    void RenderUI();

    void SetupDockspace();

    void Render();


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

    int m_systemInfoCount = 0;
    int m_terminalCount = 0;
    int m_editHostsCount = 0;
    int m_contentExplorerCount = 0;
    int m_debugWindowCount = 0;
    int m_sqlWindowCount =0;

    //Updatables
    std::vector<std::shared_ptr<Updatable>> m_updatables = {};
#ifdef WITHHUMMINGBIRDKASPERSPECIFIC
    int m_visualLinkLauncherCount = 0;
#endif
  };

}// namespace HummingBirdCore