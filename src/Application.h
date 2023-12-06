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
#include "Rendering/Texture.h"


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
    GLuint SDLSurfaceToOpenGLTexture(SDL_Surface* surface);
public:
    static SDL_Window* GetWindow() { return s_window; }
private:
    std::map<std::string, std::shared_ptr<UIWindow>> m_uiWindows;
    inline static SDL_Window* s_window;

    SDL_GLContext m_gl_context{};
    bool m_exit = false;

    GLsizei m_windowWidth = 1920;
    GLsizei m_windowHeight = 1080;

    Texture m_texture = Texture("Assets/Textures/newbg.png");

};


#endif //KBTOOLS_APPLICATION_H
}