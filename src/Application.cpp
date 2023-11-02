//
// Created by Kasper de Bruin on 01/11/2023.
//

#include "Application.h"
#include "Log.h"

namespace KBTools {
    Application::Application() {
        Log::Init();
        InitSDL();
        InitImGui();
        Themes::ThemeManager::SetTheme(Themes::THEMES::MAYA);
        Run();
    }

    Application::~Application() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();

        SDL_GL_DeleteContext(m_gl_context);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }

    void Application::InitSDL() {
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
        {
            CORE_ERROR("Failed to initialize SDL: {0}", SDL_GetError());
            exit(-1);
        }

        SDL_version compiled;
        SDL_VERSION(&compiled);
        SDL_version linked;
        SDL_GetVersion(&linked);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

        SDL_GL_SetAttribute(
                SDL_GL_CONTEXT_PROFILE_MASK,
                SDL_GL_CONTEXT_PROFILE_CORE
        );

        SDL_GL_SetAttribute( // required on Mac OS
                SDL_GL_CONTEXT_FLAGS,
                SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG
        );
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

        auto window_flags = (SDL_WindowFlags)(
                SDL_WINDOW_OPENGL
                | SDL_WINDOW_RESIZABLE
                | SDL_WINDOW_ALLOW_HIGHDPI
        );
        m_window = SDL_CreateWindow(
                "Dear ImGui SDL",
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                m_windowWidth,
                m_windowHeight,
                window_flags
        );
        // limit to which minimum size user can resize the window
        SDL_SetWindowMinimumSize(m_window, 500, 300);

        m_gl_context = SDL_GL_CreateContext(m_window);
        if (m_gl_context == nullptr)
        {
            CORE_ERROR("Failed to create a GL context: {0}", SDL_GetError());
            exit(-1);
        }
        SDL_GL_MakeCurrent(m_window, m_gl_context);

        // enable VSync
        SDL_GL_SetSwapInterval(1);

        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
        {
            CORE_ERROR("Couldn't initialize glad");
            exit(-1);
        }
        else
        {
            CORE_INFO("glad initialized");
        }

        CORE_INFO("OpenGL from glad: {0}.{1}", GLVersion.major, GLVersion.minor);

        int sdlOpenGLmajor = 0,
                sdlOpenGLminor = 0;
        SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &sdlOpenGLmajor);
        SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &sdlOpenGLminor);
        CORE_INFO("OpenGL from SDL: {0}.{1}", sdlOpenGLmajor, sdlOpenGLminor);
        glViewport(0, 0, m_windowWidth, m_windowHeight);
    }

    void Application::InitImGui() {

        // setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

        // setup platform/renderer bindings
        ImGui_ImplSDL2_InitForOpenGL(m_window, m_gl_context);
        ImGui_ImplOpenGL3_Init("#version 330");
    }

    void Application::Run() {
        while (!m_exit) {
            Render();
        }
    }

    void Application::SetupDockspace() {
        ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
        ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBackground;

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
                    m_exit = true;
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
        // start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(m_window);
        ImGui::NewFrame();;

        SetupDockspace();

        for (auto &[name, window]: m_uiWindows) {
            if (window->IsOpen()) {
                if (ImGui::Begin(name.c_str())) {
                    window->Render();
                }
                ImGui::End();
            }
        }

        // End the dockspace
        ImGui::End();

        // rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    }

    void Application::Render() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            // without it you won't have keyboard input and other things
            ImGui_ImplSDL2_ProcessEvent(&event);
            // you might also want to check io.WantCaptureMouse and io.WantCaptureKeyboard
            // before processing events

            switch (event.type)
            {
                case SDL_QUIT:
                    m_exit = true;
                    break;

                case SDL_WINDOWEVENT:
                    switch (event.window.event)
                    {
                        case SDL_WINDOWEVENT_RESIZED:
                            m_windowWidth = event.window.data1;
                            m_windowHeight = event.window.data2;
                            CORE_INFO("Window size: {0}x{1}", m_windowWidth, m_windowHeight);
                            glViewport(0, 0, m_windowWidth, m_windowHeight);
                            break;
                    }
                    break;

                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            m_exit = true;
                            break;
                    }
                    break;
            }
        }


        RenderUI();

        SDL_GL_SwapWindow(m_window);
    }

}