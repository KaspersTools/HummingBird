//
// Created by Kasper de Bruin on 01/11/2023.
//

#include "Application.h"

namespace KBTools {
    Application::Application() {


    }

    Application::~Application() {

    }

    void Application::InitGlfw() {

    }

    void Application::InitImGui() {

    }

    void Application::Run() {

    }

    void Application::SetupDockspace() {
        ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
        ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBackground;

        ImGuiStyle &style = ImGui::GetStyle();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize); // Set the window size to the entire display size
        ImGui::SetNextWindowPos(ImVec2(0, 0)); // Set the window position to the top-left corner
        ImGui::Begin("FullscreenWindow", nullptr, window_flags);

        ImGui::PopStyleVar(3);

        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0, 0), ImGuiDockNodeFlags_None);

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Exit")) {
                    //TODO: Exit SDL
                }
                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu("Windows")){
                if(ImGui::MenuItem("Terminal")){
                    AddWindow(TerminalWindow::GetTerminalName(), std::make_shared<TerminalWindow>());
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("View")) {
                if (ImGui::BeginMenu("Styles")) {
                    if (ImGui::MenuItem("Maya")) {
                        Themes::ThemeManager::SetTheme(Themes::THEMES::MAYA);
                    }
                    if (ImGui::MenuItem("Phocus Green")) {
                        Themes::ThemeManager::SetTheme(Themes::THEMES::PHOCOSGREEN);
                    }
                    if (ImGui::MenuItem("Dark")) {
                        Themes::ThemeManager::SetTheme(Themes::THEMES::DARK);
                    }
                    if (ImGui::MenuItem("Light")) {
                        Themes::ThemeManager::SetTheme(Themes::THEMES::LIGHT);
                    }
                    if (ImGui::MenuItem("Classic")) {
                        Themes::ThemeManager::SetTheme(Themes::THEMES::CLASSIC);
                    }
                    if (ImGui::MenuItem("Monochrome")) {
                        Themes::ThemeManager::SetTheme(Themes::THEMES::MONOCHROME);
                    }
                    if (ImGui::MenuItem("The_0n3")) {
                        Themes::ThemeManager::SetTheme(Themes::THEMES::THE_0N3);
                    }
                    if (ImGui::MenuItem("ModernDarkTheme")) {
                        Themes::ThemeManager::SetTheme(Themes::THEMES::MODERNDARK);
                    }
                    if (ImGui::MenuItem("EmbraceTheDarkness")) {
                        Themes::ThemeManager::SetTheme(Themes::THEMES::EMBRACETHEDARKNESS);
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }

            // You can add more menus here...
            ImGui::EndMenuBar();
        }
    }

    void Application::RenderUI(){

        //TODO: REPLACE WITH SDL??
//        ImGui_ImplOpenGL3_NewFrame();
//        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();

        SetupDockspace();

        for (auto &[name, window]: m_uiWindows) {
            if (window->IsOpen()) {
                if (ImGui::Begin(name.c_str())) {
                    window->Render();
                }
                ImGui::End();
            }
        }

        // End the parent window
        ImGui::End();

        ImGui::Render();

        //TODO: REPLACE WITH SDL??
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void Application::Render() {
        //SDL START
        RenderUI();
    }

}