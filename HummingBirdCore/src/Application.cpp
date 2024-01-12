//
// Created by Kasper de Bruin on 01/11/2023.
//

#include "Application.h"

#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>

#include <SDL_opengl.h>

namespace HummingBirdCore {

  Application::Application() : m_backgroundTexture("Assets/Textures/newbg.png") {
    if (s_application != nullptr) {
      CORE_WARN("Application already exists!");
      return;
    }

    CORE_INFO("Starting HummingBirdCore Application");

    s_application = this;

    InitSDL();
    InitImGui();
    m_backgroundTexture.load();

    Themes::ApplyTheme(Themes::ImGuiTheme_PhotoshopStyle);

    Run();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(m_gl_context);
    SDL_DestroyWindow(s_window);
    SDL_Quit();
  }

  Application::~Application() {
    CORE_INFO("Closing HummingBirdCore Application");
  }

  void Application::InitSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
      CORE_ERROR("Failed to initialize SDL: {0}", SDL_GetError());
      exit(-1);
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_GL_SetAttribute(
            SDL_GL_CONTEXT_PROFILE_MASK,
            SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(// required on Apple
            SDL_GL_CONTEXT_FLAGS,
            SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    auto mainWindowFlags = (SDL_WindowFlags) (SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    s_window = SDL_CreateWindow(
            "Humming Bird",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            1280,
            720,
            mainWindowFlags);

    // limit to which minimum size user can resize the window
    SDL_SetWindowMinimumSize(s_window, 800, 600);

    m_gl_context = SDL_GL_CreateContext(s_window);
    if (m_gl_context == nullptr) {
      CORE_ERROR("Failed to create a GL context: {0}", SDL_GetError());
      exit(-1);
    }
    SDL_GL_MakeCurrent(s_window, m_gl_context);

    // enable VSync
    SDL_GL_SetSwapInterval(1);
    int sdlOpenGLmajor = 0, sdlOpenGLminor = 0;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &sdlOpenGLmajor);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &sdlOpenGLminor);
  }

  void Application::InitImGui() {

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_NavEnableKeyboard;

    io.ConfigWindowsMoveFromTitleBarOnly = true;

    io.ConfigViewportsNoAutoMerge = true;
    io.ConfigViewportsNoTaskBarIcon = false;

    io.ConfigInputTextCursorBlink = true;

#ifdef __APPLE__
    io.ConfigMacOSXBehaviors = true;
#else
    io.ConfigMacOSXBehaviors = false;
#endif

    // setup platform/renderer bindings
    ImGui_ImplSDL2_InitForOpenGL(s_window, m_gl_context);
    ImGui_ImplOpenGL3_Init("#version 330");

    ImFontConfig config;
    config.OversampleH = 3;
    config.OversampleV = 3;
    config.PixelSnapH = true;

    io.FontDefault = io.Fonts->AddFontFromFileTTF(
            "Assets/Fonts/JetBrainsMono/JetBrainsMonoNerdFontPropo-Regular.ttf", 15.0f, &config);
  }

  void Application::Run() {
    while (!m_exit) {
      Render();
    }
  }

  void Application::SetupDockspace() {
    ImGuiID dockspaceId = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspaceId, ImVec2(0, 0),
                     ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode);

    //File menu
    if (ImGui::BeginMenuBar()) {
      // File menu
      if (ImGui::BeginMenu("File")) {
        // Project management
        if (ImGui::MenuItem("New Project")) {
          CORE_WARN("NEW PROJECT NOT IMPLEMENTED");
        }
        if (ImGui::MenuItem("Open Project")) {
          CORE_WARN("OPEN PROJECT NOT IMPLEMENTED");
        }
        if (ImGui::MenuItem("Save Project")) {
          CORE_WARN("SAVE PROJECT NOT IMPLEMENTED");
        }

        ImGui::Separator();

        // Session management
        if (ImGui::MenuItem("Log Out")) { Security::LoginManager::logout(); }
        if (ImGui::MenuItem("Exit")) { m_exit = true; }
        ImGui::EndMenu();
      }

      // Tools menu
      if (ImGui::BeginMenu("Tools")) {
        // System Tools
        if (ImGui::BeginMenu("System Tools")) {
          if (ImGui::MenuItem("System Info")) {
            const std::string baseName = "System Info ";
            if (!openClosedWindow(baseName)) {
              const std::string name = baseName + std::to_string(m_systemInfoCount);

              AddWindow(name, std::make_shared<HummingBirdCore::System::SysInfoWindow>(name));
              m_systemInfoCount++;
            }
          }
          if (ImGui::MenuItem("Edit Hosts")) {
            const std::string baseName = "Edit Hosts ";
            if (!openClosedWindow(baseName)) {
              const std::string name = baseName + std::to_string(m_editHostsCount);
              AddWindow(name, std::make_shared<HummingBirdCore::System::EditHostsWindow>(name));
              m_editHostsCount++;
            }
          }

          if (ImGui::MenuItem("Launch daemons")) {
            const std::string baseName = "Launch daemons ";
            if (!openClosedWindow(baseName)) {
              const std::string name = baseName + std::to_string(m_launchDaemonsManagerCount);
              AddWindow(name, std::make_shared<HummingBirdCore::System::LaunchDaemonsManager>(name));
              m_editHostsCount++;
            }
          }

          ImGui::EndMenu();
        }

        // Developer Tools
        if (ImGui::BeginMenu("Developer Tools")) {
          if (ImGui::MenuItem("Terminal")) {
            const std::string baseName = "Terminal ";
            if (!openClosedWindow(baseName)) {
              const std::string name = baseName + std::to_string(m_terminalCount);
              AddWindow(name, std::make_shared<HummingBirdCore::Terminal::TerminalWindow>(name));
              m_terminalCount++;
            }
          }
          if (ImGui::MenuItem("Sql")) {
            const std::string baseName = "Sql ";
            if (!openClosedWindow(baseName)) {
              const std::string name = baseName + std::to_string(m_sqlWindowCount);
              AddWindow(name, std::make_shared<HummingBirdCore::Sql::SqlWindow>(name));
              m_sqlWindowCount++;
            }
          }
          if (ImGui::MenuItem("Debug Window")) {
            const std::string baseName = "Debug Window ";
            if (!openClosedWindow(baseName)) {
              const std::string name = baseName + std::to_string(m_debugWindowCount);
              AddWindow(name, std::make_shared<HummingBirdCore::UIWindows::LogWindow>(name));
              m_debugWindowCount++;
            }
          }
          ImGui::EndMenu();
        }

        // Networking
        if (ImGui::MenuItem("Networking")) {
          CORE_WARN("NETWORKING NOT IMPLEMENTED");
        }

        // Additional Tools
        if (ImGui::BeginMenu("Additional Tools")) {
          if (ImGui::MenuItem("Data Viewer")) {
            const std::string baseName = "Data Viewer ";
            if (!openClosedWindow(baseName)) {
              const std::string name = baseName + std::to_string(m_dataViewerCount);
              AddWindow(name, std::make_shared<HummingBirdCore::Widgets::DataViewer>(name));
              m_dataViewerCount++;
            }
          }

          if (ImGui::MenuItem("Content Explorer")) {
            const std::string baseName = "Content Explorer ";
            if (!openClosedWindow(baseName)) {
              const std::string name = baseName + std::to_string(m_contentExplorerCount);
              AddWindow(name, std::make_shared<HummingBirdCore::UIWindows::ContentExplorer>(name));
              m_contentExplorerCount++;
            }
          }
          ImGui::EndMenu();
        }

        ImGui::EndMenu();
      }

      //View menu
      if (ImGui::BeginMenu("View")) {
        if (ImGui::BeginMenu("Styles")) {
          if (ImGui::MenuItem("ThemeManager")) {
            const std::string baseName = "ThemeManager ";
            if (!openClosedWindow(baseName)) {
              const std::string name = baseName + std::to_string(m_themeManagerCount);
              AddWindow(name, std::make_shared<Themes::ThemeManager>(name));
              m_themeManagerCount++;
            }
          }
          if (ImGui::BeginMenu("Themes")) {
            if (ImGui::MenuItem("ImGuiColorsClassic")) {
              Themes::ApplyTheme(Themes::ImGuiTheme_ImGuiColorsClassic);
            }
            if (ImGui::MenuItem("ImGuiColorsDark")) {
              Themes::ApplyTheme(Themes::ImGuiTheme_ImGuiColorsDark);
            }
            if (ImGui::MenuItem("ImGuiColorsLight")) {
              Themes::ApplyTheme(Themes::ImGuiTheme_ImGuiColorsLight);
            }
            if (ImGui::MenuItem("MaterialFlat")) {
              Themes::ApplyTheme(Themes::ImGuiTheme_MaterialFlat);
            }
            if (ImGui::MenuItem("PhotoshopStyle")) {
              Themes::ApplyTheme(Themes::ImGuiTheme_PhotoshopStyle);
            }
            if (ImGui::MenuItem("GrayVariations")) {
              Themes::ApplyTheme(Themes::ImGuiTheme_GrayVariations);
            }
            if (ImGui::MenuItem("GrayVariations_Darker")) {
              Themes::ApplyTheme(Themes::ImGuiTheme_GrayVariations_Darker);
            }
            if (ImGui::MenuItem("MicrosoftStyle")) {
              Themes::ApplyTheme(Themes::ImGuiTheme_MicrosoftStyle);
            }
            if (ImGui::MenuItem("Cherry")) {
              Themes::ApplyTheme(Themes::ImGuiTheme_Cherry);
            }
            if (ImGui::MenuItem("Darcula")) {
              Themes::ApplyTheme(Themes::ImGuiTheme_Darcula);
            }
            if (ImGui::MenuItem("DarculaDarker")) {
              Themes::ApplyTheme(Themes::ImGuiTheme_DarculaDarker);
            }
            if (ImGui::MenuItem("LightRounded")) {
              Themes::ApplyTheme(Themes::ImGuiTheme_LightRounded);
            }
            if (ImGui::MenuItem("SoDark_AccentBlue")) {
              Themes::ApplyTheme(Themes::ImGuiTheme_SoDark_AccentBlue);
            }
            if (ImGui::MenuItem("SoDark_AccentYellow")) {
              Themes::ApplyTheme(Themes::ImGuiTheme_SoDark_AccentYellow);
            }
            if (ImGui::MenuItem("SoDark_AccentRed")) {
              Themes::ApplyTheme(Themes::ImGuiTheme_SoDark_AccentRed);
            }
            if (ImGui::MenuItem("BlackIsBlack")) {
              Themes::ApplyTheme(Themes::ImGuiTheme_BlackIsBlack);
            }
            if (ImGui::MenuItem("WhiteIsWhite")) {
              Themes::ApplyTheme(Themes::ImGuiTheme_WhiteIsWhite);
            }
            ImGui::EndMenu();
          }
          ImGui::EndMenu();
        }
        ImGui::EndMenu();
      }

#ifdef WITHHUMMINGBIRDKASPERSPECIFIC
      if (ImGui::BeginMenu("Kasper Tools")) {
        if (ImGui::MenuItem("Hello World")) {
        }
        if (ImGui::BeginMenu("Visual Link")) {
          if (ImGui::MenuItem("Visual Link Launcher")) {
            AddWindow("Visual Link", std::make_shared<HummingBirdKasper::VisualLink::VisualLinkLauncher>());
          }

          ImGui::EndMenu();
        }

        ImGui::EndMenu();
      }
#endif
      if (ImGui::BeginMenu("Third Party")) {
        if (ImGui::BeginMenu("ImGui")) {
          if (ImGui::MenuItem("Demo Window")) {
            m_showDemoWindow = true;
          }
          ImGui::EndMenu();
        }
        ImGui::EndMenu();
      }

      ImGui::EndMenuBar();
    }
  }

  void Application::RenderUI() {
    {
      ImGuiWindowFlags mainWindowFlags =
              ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
              ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
              ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBackground;

      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

      ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);// Set the window size to the entire display size
      ImGui::SetNextWindowPos(ImVec2(0, 0));               // Set the window position to the top-left corner
      ImGui::Begin("FullscreenWindow", nullptr, mainWindowFlags);

      ImGui::PopStyleVar(3);
    }

    //Render background texture
    {
      GLuint textureID = m_backgroundTexture.getTextureID();// Replace with your actual texture ID
      ImVec2 texSize = ImVec2(
              getWindowWidth(),
              getWindowHeight());
      ImGui::GetWindowDrawList()->AddImage(
              (void *) (intptr_t) textureID,
              ImVec2(ImGui::GetWindowPos()),
              ImVec2(ImGui::GetWindowPos().x + texSize.x, ImGui::GetWindowPos().y + texSize.y),
              ImVec2(0, 0), ImVec2(1, 1));
    }

    SetupDockspace();

    {
      if (!Security::LoginManager::isLoggedIn()) {
        m_loginWindow.render();
      } else {
        if (m_showDemoWindow)
          ImGui::ShowDemoWindow(&m_showDemoWindow);

        for (auto window: m_uiWindows) {
          window.second->beginFrame();
        }
      }
    }

    ImGui::End();
  }

  void Application::Render() {

    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL2_ProcessEvent(&event);
      if (event.type == SDL_QUIT)
        m_exit = true;
      if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(s_window))
        m_exit = true;
    }

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    RenderUI();
    //Render toast notifications
    {
      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f);
      ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(43.f / 255.f, 43.f / 255.f, 43.f / 255.f, 100.f / 255.f));
      ImGui::RenderNotifications();
      ImGui::PopStyleVar(1);// Don't forget to Pop()
      ImGui::PopStyleColor(1);
    }

    ImGui::Render();

    glViewport(0, 0, (int) GetWindowWidth(), (int) GetWindowHeight());
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(s_window);
  }
}// namespace HummingBirdCore
