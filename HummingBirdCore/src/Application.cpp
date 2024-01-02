//
// Created by Kasper de Bruin on 01/11/2023.
//
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Application.h"
#include "Log.h"

namespace HummingBirdCore {

  Application::Application() {
    if (s_application != nullptr) {
      CORE_WARN("Application already exists!");
      return;
    }

    CORE_INFO("Starting HummingBirdCore Application");

    s_application = this;

    InitSDL();
    InitImGui();

    Themes::ThemeManager::SetTheme(Themes::THEMES::PHOCOSGREEN);

    m_backgroundTexture.load();

    //Test mysql connection
    //    std::string host = "127.0.0.1";
    //    std::string username = "root";
    //    std::string passw = "Vuur01-";
    //    std::string database = "dsu_core";
    //    unsigned int port = 3306;
    //
    //    Sql::SqlConnection sqlConnection(host, username, passw, port);
    //
    //    if (sqlConnection.isConnected()) {
    //      CORE_TRACE("Connected to database");
    //    } else {
    //      CORE_ERROR("Failed to connect to database");
    //    }


    Run();
  }

  Application::~Application() {
    CORE_INFO("Closing HummingBirdCore Application");

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(m_gl_context);
    SDL_DestroyWindow(s_window);
    SDL_Quit();
  }

  void Application::InitSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
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
            SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(// required on Mac OS
            SDL_GL_CONTEXT_FLAGS,
            SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    //    SDL_WINDOW_ALLOW_HIGHDPI

    auto mainWindowFlags = (SDL_WindowFlags) (SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    s_window = SDL_CreateWindow(
            "Humming Bird",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            m_windowWidth,
            m_windowHeight,
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

    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
      CORE_ERROR("Couldn't initialize glad");
      exit(-1);
    } else {
      CORE_INFO("glad initialized");
    }

    CORE_INFO("OpenGL from glad: {0}.{1}", GLVersion.major, GLVersion.minor);

    int sdlOpenGLmajor = 0, sdlOpenGLminor = 0;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &sdlOpenGLmajor);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &sdlOpenGLminor);
    CORE_INFO("OpenGL from SDL: {0}.{1}", sdlOpenGLmajor, sdlOpenGLminor);
    glViewport(0, 0, m_windowWidth, m_windowHeight);
  }

  void Application::InitImGui() {

    // setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    // setup platform/renderer bindings
    ImGui_ImplSDL2_InitForOpenGL(s_window, m_gl_context);
    ImGui_ImplOpenGL3_Init("#version 330");

    ImFontConfig config;
    config.OversampleH = 3;
    config.OversampleV = 3;
    config.PixelSnapH = true;

    io.FontDefault = io.Fonts->AddFontFromFileTTF(
            "Assets/Fonts/JetBrainsMono/JetBrainsMonoNerdFontPropo-Regular.ttf", 16.0f, &config);
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
            if (ImGui::MenuItem("DoughBkins_Black")) {
              Themes::ThemeManager::SetTheme(Themes::THEMES::DOUGHBKINS_BLACK);
            }
            if (ImGui::MenuItem("DoughBkins_White")) {
              Themes::ThemeManager::SetTheme(Themes::THEMES::DOUGHBKINS_WHITE);
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
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(s_window);
    ImGui::NewFrame();

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
              m_windowWidth,
              m_windowHeight);
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

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  }

  void Application::Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL2_ProcessEvent(&event);

      Input::update(event);

      switch (event.type) {
        case SDL_QUIT:
          m_exit = true;
          break;

        case SDL_WINDOWEVENT:
          switch (event.window.event) {
            case SDL_WINDOWEVENT_RESIZED:
              m_windowWidth = event.window.data1;
              m_windowHeight = event.window.data2;
              glViewport(0, 0, m_windowWidth, m_windowHeight);
              break;
          }
          break;

        case SDL_KEYDOWN:
          switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
              m_exit = true;
              break;
          }
          break;
      }
    }

    RenderUI();


    SDL_GL_SwapWindow(s_window);
  }
}// namespace HummingBirdCore
