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

    //Load fonts

    ImFontConfig config;
    config.OversampleH = 3;
    config.OversampleV = 3;

    ImFont* mainFont = io.Fonts->AddFontFromFileTTF(
            "Assets/Fonts/JetBrainsMono/JetBrainsMonoNerdFontPropo-Regular.ttf",
            17, &config);
    io.FontDefault = mainFont;

}

void Application::Run(){
    while(!glfwWindowShouldClose(m_nativeWindow)){
        Render();
    }
}

void Application::SetupDockspace() {
    // 2. Start the ImGui frame
//    ImGuiStyle& style = ImGui::GetStyle();
//    ImVec2 windowPadding = style.WindowPadding; // Store original padding
//    ImVec2 itemSpacing =   style.ItemSpacing; // Store original item spacing
//    ImVec2 framePadding =  style.FramePadding; // Store original frame padding
//
//    style.WindowPadding = ImVec2(0.0f, 0.0f);  // Set padding to zero for this window
//    style.ItemSpacing = ImVec2(0.0f, 0.0f);    // Optional: reduce spacing between items
//    style.FramePadding = ImVec2(0.0f, 0.0f);   // Optional: reduce frame padding
//    //Render imgui
//
//    // Window flags for the fullscreen window
//    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
//    window_flags |= ImGuiWindowFlags_NoTitleBar;
//    window_flags |= ImGuiWindowFlags_NoCollapse;
//    window_flags |= ImGuiWindowFlags_NoResize;
//    window_flags |= ImGuiWindowFlags_NoMove;
//    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
//    window_flags |= ImGuiWindowFlags_NoNavFocus;
//    window_flags |= ImGuiWindowFlags_NoBackground; ;  // Disables the close button
//
//
//    // Make it fullscreen
//    const ImGuiViewport* viewport = ImGui::GetMainViewport();
//    ImGui::SetNextWindowPos(viewport->Pos);
//    ImGui::SetNextWindowSize(viewport->Size);
//    ImGui::SetNextWindowViewport(viewport->ID);
//// Begin the parent window
//    ImGui::Begin("DockSpace Demo", nullptr, window_flags);
//
//    // Restore original style after you're done with the fullscreen window
//    style.FramePadding.x  = framePadding.x;
//    style.WindowPadding.x =  windowPadding.x;
//    style.ItemSpacing.x   = itemSpacing.x;
//
//    // Custom taskbar (menu bar)
//    if (ImGui::BeginMenuBar()) {
//        if (ImGui::BeginMenu("File")) {
//            if (ImGui::MenuItem("New")) {
//                // Handle "New" action here
//            }
//            if (ImGui::MenuItem("Open")) {
//                // Handle "Open" action here
//            }
//            if (ImGui::MenuItem("Save")) {
//                // Handle "Save" action here
//            }
//            if (ImGui::MenuItem("Exit")) {
//                glfwSetWindowShouldClose(m_nativeWindow, true); // Close the application
//            }
//            ImGui::EndMenu();
//        }
//        if (ImGui::BeginMenu("Edit")) {
//            if (ImGui::MenuItem("Undo")) {
//                // Handle "Undo" action here
//            }
//            // ... add other Edit options as needed ...
//            ImGui::EndMenu();
//        }
//        // ... add other menus as needed ...
//        ImGui::EndMenuBar();
//    }
//
//
//    // DockSpace
//    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
//    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
    window_flags |= ImGuiWindowFlags_NoNavFocus;
    window_flags |= ImGuiWindowFlags_NoBackground; ;  // Disables the close button

    ImGuiStyle& style = ImGui::GetStyle();
    ImVec2 ogWindowPadding = style.WindowPadding;
    style.WindowPadding = ImVec2(0.0f, 0.0f);  // Set padding to zero for this window

    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize); // Set the window size to the entire display size
    ImGui::SetNextWindowPos(ImVec2(0, 0)); // Set the window position to the top-left corner
    ImGui::Begin("FullscreenWindow", nullptr, window_flags);
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0,0), ImGuiDockNodeFlags_None);

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit"))
            {
                // Handle exit logic here
            }
            ImGui::EndMenu();
        }
        // You can add more menus here...
        ImGui::EndMenuBar();
    }
    style.WindowPadding = ogWindowPadding;
}

void Application::Render() {
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();

    SetupDockspace();

    for (auto& [name, window] : m_uiWindows) {
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
    int display_w, display_h;
    glfwGetFramebufferSize(m_nativeWindow, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(m_nativeWindow);
}

