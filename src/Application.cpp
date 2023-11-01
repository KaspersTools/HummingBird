//
// Created by Kasper de Bruin on 01/11/2023.
//

#include "Application.h"

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

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }

}

void Application::InitImGui() {

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();


    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Setup ImGui platform/renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(m_nativeWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330");

}

void Application::Run(){
    while(!glfwWindowShouldClose(m_nativeWindow)){
        Render();
    }
}

void Application::Render() {
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();

    for (auto& [name, window] : windows) {
        if (window->IsOpen()) {
            if (ImGui::Begin(name.c_str())) {
                window->Render();
            }
            ImGui::End();
        }
    }
//    ImGui::Begin("t");
//    ImGui::Text("teszt");
//    ImGui::End();
//
//    ImGui::ShowDemoWindow();

    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(m_nativeWindow, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(m_nativeWindow);
}

