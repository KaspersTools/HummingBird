//
// Created by Kasper de Bruin on 01/11/2023.
//

#ifndef KBTOOLS_APPLICATION_H
#define KBTOOLS_APPLICATION_H

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "spdlog/logger.h"

#include "UIWindows/UIWindow.h"
#include "UIWindows/TerminalWindow.h"


#include <imgui.h>
#include <iostream>
#include <map>
#include <string>
#include <memory>


class Application {
public:
    Application();
    ~Application();

    void InitGlfw();
    void InitImGui();

    void AddWindow(const std::string& name, std::shared_ptr<UIWindow> window) {
        m_uiWindows[name] = window;
    }

    void RemoveWindow(const std::string& name) {
        m_uiWindows.erase(name);
    }



private:
    void Run();
    void Render();


private:

    GLFWwindow* m_nativeWindow;

    std::map<std::string, std::shared_ptr<UIWindow>> m_uiWindows;

};


#endif //KBTOOLS_APPLICATION_H
