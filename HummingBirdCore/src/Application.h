//
//
// Created by Kasper de Bruin on 01/11/2023.
//

#pragma once

#include <iostream>
#include <map>
#include <string>
#include <memory>

#include <vector>
#include <iostream>


// Dear ImGui
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>

// SDL
#include <SDL.H>
#include <glad/glad.h>


// spdlog
#include <spdlog/spdlog.h>

// UIWindows
#include "UIWindows/UIWindow.h"

#include "Terminal/TerminalWindow.h"

#include "Security/LogInWindow.h"
#include "Security/LoginManager.h"

#include "GeneralTools/TextEditting/TextEditorWindow.h"
#include "GeneralTools/FileDialog.h"

#include "System/Hosts/EditHostsWindow.h"

#include "UIWindows/Themes/ThemeManager.h"

// Rendering
#include "Rendering/Texture.h"

// Sql
#include "Sql/SqlManager.h"

namespace HummingBirdCore {
  class Application {
public:
    Application();

    ~Application();

    void AddWindow(const std::string &name, std::shared_ptr<UIWindow> uiWindow) {
      m_uiWindows[name] = uiWindow;
    }

    void RemoveWindow(const std::string &name) {
      m_uiWindows.erase(name);
    }


    const HummingBirdCore::Sql::SqlManager &GetSqlManager() const {
      return m_sqlManager;
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

    // Sql Manager
    const HummingBirdCore::Sql::SqlManager m_sqlManager;


    //imgui third party windows
    bool m_showDemoWindow = false;
    bool m_showMetricsWindow = false;
    bool m_showStyleEditor = false;
    bool m_showAboutWindow = false;
    bool m_ShowExampleAppDocuments = false;
    bool m_ShowExampleAppConsole = false;
    bool m_ShowExampleAppLog = false;
    bool m_ShowExampleAppLayout = false;
//    bool m_ShowExampleAppLongText = false;
//    bool m_ShowExampleAppAutoResize = false;
//
  };

}// namespace KBTools