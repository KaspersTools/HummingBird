//
// Created by Kasper de Bruin on 06/12/2023.
//

#include "LogInWindow.h"
#include "../Application.h"

namespace KBTools::Security {

    void KBTools::Security::LogInWindow::Render() {
        //Center the window
        Application* app = KBTools::Application::GetApplication();


        ImGui::SetNextWindowSize(ImVec2(400, 200));

        //Center the window
        ImGui::SetNextWindowPos(ImVec2(app->GetWindowWidth() / 2 - 200, app->GetWindowHeight() / 2 - 100));

        ImGuiWindowFlags flags =
                ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings |
                ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

        ImGui::Begin("Login Panel", &isOpen, flags);
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Username:");
        ImGui::InputText("##username", m_username, sizeof(m_username), 0, 0, 0);
        ImGui::Text("Password:");
        ImGui::InputText("##password", m_password, sizeof(m_password), 0, 0, 0);
        ImGui::Checkbox("Remember login details", &m_remember);

        if(ImGui::Button("Login")){
            if(LoginManager::Login(m_username, m_password)) {
                isOpen = false;
            }
        }

        ImGui::End();


//        ImGui::Begin("Log In", &isOpen,
//                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
//
//        ImGui::Text("Hello, world!");
//
//        if (ImGui::Button("LOGIN")) {
//
//        };
//        ImGui::End();
    }

    LogInWindow::LogInWindow() {

    }

    LogInWindow::~LogInWindow() {

    }
} // Security