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
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"


#include "spdlog/logger.h"

#include "UIWindows/UIWindow.h"
#include "UIWindows/TerminalWindow.h"

#include "UIWindows/Themes/ThemeManager.h"


namespace KBTools{
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


private:
    void InitSDL();
    void InitImGui();
    void Run();

    void RenderUI();
    void SetupDockspace();

    void Render();

private:
    std::map<std::string, std::shared_ptr<UIWindow>> m_uiWindows;
    SDL_Window* m_window{};
    SDL_GLContext m_gl_context{};
    bool m_exit = false;
    bool m_show_demo_window = false;
    bool m_show_another_window = false;

    GLsizei m_windowWidth = 1280;
    GLsizei m_windowHeight = 720;
};


#endif //KBTOOLS_APPLICATION_H
}