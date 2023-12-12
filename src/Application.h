//
// Created by Kasper de Bruin on 01/11/2023.
//

#ifndef KBTOOLS_APPLICATION_H
#define KBTOOLS_APPLICATION_H

#include <iostream>
#include <map>
#include <string>
#include <memory>


#include <vector>
#include <iostream>
// SDL
#include <glad/glad.h>
#include <SDL.h>

// Dear ImGui
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>

// spdlog
#include "spdlog/logger.h"

// UIWindows
#include "UIWindows/UIWindow.h"
#include "Terminal/TerminalWindow.h"
#include "Security/LogInWindow.h"
#include "Security/LoginManager.h"
#include "GeneralTools/TextEditting/TextEditorWindow.h"

#include "UIWindows/Themes/ThemeManager.h"

// Rendering
#include "Rendering/Texture.h"

// Sql
#include "Sql/SqlManager.h"

namespace KBTools {
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


    const HummingBird::Sql::SqlManager &GetSqlManager() const {
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
    const HummingBird::Sql::SqlManager m_sqlManager;
  };


#endif //KBTOOLS_APPLICATION_H

}// namespace KBTools