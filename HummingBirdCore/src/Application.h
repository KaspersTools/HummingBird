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
#include "UIWindows/Widget/DataViewer.h"
#include "UIWindows/Widget/MetricsWidget.h"

// OTHER WINDOWS
#include "Terminal/TerminalWindow.h"

#include "Security/LogInWindow.h"
#include "Security/LoginManager.h"

#include "System/Hosts/EditHostsWindow.h"
#include "System/SysInfoWindow.h"

#include "Sql/SqlWindow.h"

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

namespace HummingBirdCore {
  class Application {
public:
    Application();

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

    static SDL_Window *GetWindow() { return s_window; }
    static Application *GetApplication() {
      return s_application;
    }

    ImVec2 getWindowSize() {
      int w, h;
      SDL_GetWindowSize(s_window, &w, &h);
      return{ (float)w, (float)h };
    }

    float getWindowWidth() {
      return getWindowSize().x;
    }

    float getWindowHeight(){
        return getWindowSize().y;
    }
private:
    void InitSDL();

    void InitImGui();

    void Run();

    void RenderUI();

    void SetupDockspace();

    void Render();

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
    inline static SDL_Window *s_window;
    SDL_GLContext m_gl_context{};

    inline static Application *s_application = nullptr;
    bool m_exit = false;

    // UIWindows
    std::map<std::string, std::shared_ptr<UIWindow>> m_uiWindows;
    Security::LogInWindow m_loginWindow;

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

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    Texture m_backgroundTexture;

    //Updatables
    std::vector<std::shared_ptr<Updatable>> m_updatables = {};
#ifdef WITHHUMMINGBIRDKASPERSPECIFIC
    int m_visualLinkLauncherCount = 0;
#endif
  };

}// namespace HummingBirdCore