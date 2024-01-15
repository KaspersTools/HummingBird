//
//
// Created by Kasper de Bruin on 01/11/2023.
//

#pragma once
#include <PCH/pch.h>

// UIWindows
#include "UIWindows/ContentExplorer.h"
#include "UIWindows/LogWindow.h"
#include "UIWindows/UIWindow.h"

// UIWindows/Themes
#include "UIWindows/Themes/ThemeManager.h"
#include "UIWindows/Themes/Themes.h"

// UIWindows/Widget
#include "UIWindows/Widget/AllAddonsWidget.h"
#include "UIWindows/Widget/DataViewer.h"
#include "UIWindows/Widget/MetricsWidget.h"

// OTHER WINDOWS
#include "Terminal/TerminalWindow.h"

#include "Security/LogInWindow.h"
#include "Security/LoginManager.h"

#include "System/Hosts/EditHostsWindow.h"
#include "System/LaunchDaemonsManager.h"
#include "System/SysInfoWindow.h"


#include "Sql/SqlWindow.h"
#include "Sql/SqlImportWindow.h"

// Rendering
#include "Rendering/Texture.h"

// System
#include "System/SysInfo.h"

// Kasper specific
#ifdef WITHHUMMINGBIRDKASPERSPECIFIC
#include <Hellebrekers/VisualLinkLauncher.h>
#include <HelloLibrary.h>

namespace HummingBirdKasper::VisualLink {
  class VisualLinkLauncher;
}
#endif

#include <GLFW/glfw3.h>// Will drag system OpenGL headers

namespace HummingBirdCore {
  class Application {
public:
    Application();
    int InitializeAndRun();
    ~Application();

    //TODO: find more intuitive way to add windows
    UIWindow &AddWindow(const std::string &name, std::shared_ptr<UIWindow> uiWindow) {
      m_uiWindows[name] = uiWindow;
      return *uiWindow;
    }

    void AddUpdatable(std::shared_ptr<Updatable> updatable) {
      m_updatables.emplace_back(updatable);
    }


    //Getters
    const float GetWindowWidth() const {
      ImGuiIO &io = ImGui::GetIO();
      return io.DisplaySize.x;
    }
    const int GetWindowHeight() const {
      ImGuiIO &io = ImGui::GetIO();
      return io.DisplaySize.y;
    }

    ImVec2 getWindowSize() {
      int w, h;
      //      SDL_GetWindowSize(s_window, &w, &h);
      glfwGetWindowSize(s_window, &w, &h);
      return {(float) w, (float) h};
    }

    float getWindowWidth() {
      return getWindowSize().x;
    }

    float getWindowHeight() {
      return getWindowSize().y;
    }

    void addMenuItem(const std::string &name, const std::function<void()> &callback) {
      m_menuItems.emplace_back(MenuItem{name, callback});
    }

private:
    void InitGlfw();

    void InitImGui();

    void Run();

    void RenderUI();

    void RenderMenuBar();

    void Render();

    void BeginFullScreenWindow();

    inline static void glfwErrorCallback(int error, const char *description) {
      CORE_ERROR("Error " + std::to_string(error) + ": " + description);
    }

    bool openClosedWindow(const std::string &baseName) {
      for (auto &window: m_uiWindows) {
        if (window.first.find(baseName) == 0) {
          if (!window.second->isOpen()) {
            window.second->m_isOpen = true;
            return true;
          }
        }
      }
      return false;
    }



private:
    inline static GLFWwindow *s_window;
    inline static Application *s_application = nullptr;

    std::vector<HummingBirdCore::MenuItem> m_menuItems = {};
    std::string glslVersion;

    bool m_exit = false;

    // UIWindows
    std::map<std::string, std::shared_ptr<UIWindow>> m_uiWindows;
    Security::LogInWindow m_loginWindow;

    int m_windowWidth;
    int m_windowHeight;
    const float c_toolbarHeight = 50.0f;
    float m_menuBarHeight = 0.0f;

    bool renderDockspace = true;

    //imgui third party windows
    bool m_showDemoWindow = false;
    bool m_showMetricsWindow = false;

    int m_systemInfoCount = 0;
    int m_terminalCount = 0;
    int m_editHostsCount = 0;
    int m_contentExplorerCount = 0;
    int m_debugWindowCount = 0;
    int m_sqlWindowCount = 0;
    int m_themeManagerCount = 0;
    int m_dataViewerCount = 0;
    int m_launchDaemonsManagerCount = 0;
    int m_sqlImportWindowCount = 0;

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    Texture m_backgroundTexture;

    //Updatables
    std::vector<std::shared_ptr<Updatable>> m_updatables = {};
#ifdef WITHHUMMINGBIRDKASPERSPECIFIC
    int m_visualLinkLauncherCount = 0;
#endif
  };

}// namespace HummingBirdCore