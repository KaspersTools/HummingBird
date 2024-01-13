//
// Created by Kasper de Bruin on 01/11/2023.
//

#include "Application.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


namespace HummingBirdCore {

  Application::Application() : m_backgroundTexture("Assets/Textures/newbg.png") {
    if (s_application != nullptr) {
      CORE_WARN("Application already exists!");
      return;
    }

    CORE_INFO("Starting HummingBirdCore Application");

    s_application = this;

    InitGlfw();

    InitImGui();
    m_backgroundTexture.load();

    Themes::ApplyTheme(Themes::ImGuiTheme_PhotoshopStyle);

    Run();

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(s_window);
    glfwTerminate();
  }

  Application::~Application() {
    CORE_INFO("Closing HummingBirdCore Application");
  }

  void Application::InitGlfw() {
    glfwSetErrorCallback(glfwErrorCallback);
    if (!glfwInit()) {
      CORE_ERROR("Failed to initialize GLFW");
      exit(-1);
    }

    // GL 3.2 + GLSL 150
    glslVersion = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);// 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);          // Required on Mac


    // Create window with graphics context
    s_window = glfwCreateWindow(1280, 720, "-- HummingBird Tools--", nullptr, nullptr);

    glfwMakeContextCurrent(s_window);
    glfwSwapInterval(1);// Enable vsync

    return;
  }

  void Application::InitImGui() {

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;    // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;  // Enable Multi-Viewport / Platform Windows
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;// Enable Keyboard Controls


    io.ConfigViewportsNoTaskBarIcon = false;
    io.ConfigInputTextCursorBlink = true;

#ifdef __APPLE__
    io.ConfigMacOSXBehaviors = true;
#else
    io.ConfigMacOSXBehaviors = false;
#endif

    ImGui_ImplGlfw_InitForOpenGL(s_window, true);
    ImGui_ImplOpenGL3_Init(glslVersion.c_str());

    ImFontConfig config;
    config.OversampleH = 3;
    config.OversampleV = 3;
    config.RasterizerDensity = 1.0f;

    io.FontDefault = io.Fonts->AddFontFromFileTTF(
            "Assets/Fonts/JetBrainsMono/JetBrainsMonoNerdFontPropo-Regular.ttf", 15.0f, &config);
  }

  void Application::Run() {
    while (!m_exit) {
      Render();
    }
  }

  void Application::RenderMenuBar() {
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

  void Application::BeginFullScreenWindow() {
    ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    const ImGuiViewport *viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
      window_flags |= ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    bool open = true;
    ImGui::Begin("DockSpace Demo", &open, window_flags);
    ImGui::PopStyleVar();
    ImGui::PopStyleVar(2);RenderMenuBar();

    ImGuiIO &io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
      ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
      ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    ImGui::End();
  }

  void Application::RenderUI() {
    if (renderDockspace) {
      BeginFullScreenWindow();
    }
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


  void Application::Render() {
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    RenderUI();

    {
      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f);
      ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(43.f / 255.f, 43.f / 255.f, 43.f / 255.f, 100.f / 255.f));
      ImGui::RenderNotifications();
      ImGui::PopStyleVar(1);// Don't forget to Pop()
      ImGui::PopStyleColor(1);
    }

    ImGui::Render();

    glfwGetFramebufferSize(s_window, &m_windowWidth, &m_windowHeight);
    glViewport(0, 0, m_windowWidth, m_windowHeight);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
      GLFWwindow *backup_current_context = glfwGetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      glfwMakeContextCurrent(backup_current_context);
    }

    glfwSwapBuffers(s_window);
  }
}// namespace HummingBirdCore
