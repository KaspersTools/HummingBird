//
// Created by Kasper de Bruin on 01/11/2023.
//

#include "Application.h"

namespace KBTools {
    Application::Application() {

        InitGlfw();
        InitImGui();

        AddWindow("Window 1", std::make_shared<TerminalWindow>());

        Run();

    }

    Application::~Application() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(m_nativeWindow);
        glfwTerminate();
    }

    void Application::InitGlfw() {
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            exit(-1);
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Necessary for Mac
#endif
        m_nativeWindow = glfwCreateWindow(640, 480, "ImGui Mac App", nullptr, nullptr);
        if (m_nativeWindow == nullptr) {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            exit(-1);
        }
        glfwMakeContextCurrent(m_nativeWindow);

        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            exit(-1);
        }

    }

    void Application::InitImGui() {

        // Setup ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();


        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        // Setup ImGui platform/renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(m_nativeWindow, true);
        ImGui_ImplOpenGL3_Init("#version 330");

        //Load fonts

        ImFontConfig config;
        config.OversampleH = 3;
        config.OversampleV = 3;

        ImFont *mainFont = io.Fonts->AddFontFromFileTTF(
                "Assets/Fonts/JetBrainsMono/JetBrainsMonoNerdFontPropo-Regular.ttf",
                15.5, &config);
        io.FontDefault = mainFont;

    }

    void Application::Run() {
        while (!glfwWindowShouldClose(m_nativeWindow)) {
            Render();
        }
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
                    glfwSetWindowShouldClose(m_nativeWindow, true);
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

    void Application::Render() {
        glfwPollEvents();

        int display_w, display_h;
        glfwGetFramebufferSize(m_nativeWindow, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

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

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_nativeWindow);
    }

}