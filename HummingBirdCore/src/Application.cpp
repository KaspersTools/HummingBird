//
// Created by Kasper de Bruin on 01/11/2023.
//


#include <imconfig.h>
#include <misc/cpp/imgui_stdlib.h>

#include <imgui.h>
#include <imgui_internal.h>
#include <addons/imguinotify/imgui_notify.h>

#include "Application.h"
#include <KDB_ImGui/backends/debug/imgui_impl_glfw_vulkan_debug.h>
#include <KDB_ImGui/backends/imgui_impl_glfw_vulkan_window.h>

#include <KDB_ImGui/fonts/FontManager.h>

#include <UIWindows/MainMenuBar.h>
#include <UIWindows/WindowManager.h>

#include <portable-file-dialogs.h>

#include <dlfcn.h>
#include <iostream>

#include <Sql/SqlWindow.h>

namespace HummingBirdCore {
  void Application::init() {

    handle = dlopen("/Users/k.debruin/_private/_hummingbird/workdir/HummingBird/cmake-build-debug-exe/plugins/TestPlugin/libHUMMINGBIRD_PLUGIN_TEST.dylib", RTLD_LAZY);
    if (!handle) {
      std::cerr << "Cannot load library: " << dlerror() << '\n';
      exit(-1);
    }

    // reset errors
    dlerror();

    // load the symbols
    HummingBird::Plugins::IPlugin* (*create_plugin)();
    *(void**)(&create_plugin) = dlsym(handle, "create_plugin");
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
      std::cerr << "Cannot load symbol create: " << dlsym_error << '\n';
      dlclose(handle);
      exit(-1);
    }

    // create an instance of the class
    plugin = create_plugin();

    // use the class
    plugin->initialize();


    ApplicationSpecification m_Specification;
    //App settings
    m_Specification.Name = "Hummingbird Core";
    m_Specification.WindowSettings.Width = 800;
    m_Specification.WindowSettings.Height = 600;
    m_Specification.WindowSettings.WindowResizeable = true;
    m_Specification.WindowSettings.WindowDecorated = true;
    m_Specification.WindowSettings.CenterWindow = true;
    m_Specification.WindowSettings.CreateDefaultDockSpace = true;

    //Titlebar settings
    m_Specification.TitleBarSettings.CustomTitleBar = true;
    m_Specification.TitleBarSettings.HasLogo = true;
    m_Specification.TitleBarSettings.LogoPath = "Assets/Textures/logo.png";
    m_Specification.TitleBarSettings.LogoDrawSize = {45.f, 45.f};

    m_Specification.TitleBarSettings.DrawTitleCentered = true;

    m_Specification.TitleBarSettings.MainMenuBarExtraHeight = 19.0f;
    m_Specification.TitleBarSettings.Height = 60.132f;

    m_Specification.TitleBarSettings.MainMenuBarCallback = new std::function<void()>([&]() {
      UI::mainMenuBarCallback();
    });

    ImGui_ImplVKGlfw_setImplErrorCallback([&](int error, const char *description) {
      std::cout << "GLFWVULKANIMPL::errorCallback() error: " << error << " description: " << description << std::endl;
    });

    ImGui_ImplVKGlfw_init(m_Specification);

    pfd::message("Welcome to Hummingbird Core", "Welcome to Hummingbird Core, the core of the Hummingbird Engine. "
                                               "This is a work in progress, so expect bugs and crashes. If you find any, "
                                               "please report them to the Hummingbird Github page. \n"
                                               "Enjoy!");


    //load fonts

    ImFontConfig fontConfig = ImFontConfig();

    fontConfig.RasterizerDensity = 2.0f;
    fontConfig.RasterizerMultiply = 1.0f;
    HBUI::FontManager::addFont("Assets/Fonts/JetBrainsMono/JetBrainsMonoNerdFontPropo-Regular.ttf", "JetBrainsMono", 16.0f, fontConfig);

    HummingBirdCore::UI::WindowManager *windowManager = HummingBirdCore::UI::WindowManager::getInstance();
    windowManager->addWindow("Sql Window", 0, std::make_shared<HummingBirdCore::SqlWindow>("Sql Window"));

    run();
  }

  bool Application::run() {
    while (!ImGui_ImplVKGlfw_shouldClose()) {
      render();
    }
  }

  void Application::render() {
    ImGui_ImplVKGlfw_startRender();

    ImGui::ShowDemoWindow();
    plugin->render(ImGui::GetCurrentContext());
    HummingBirdCore::UI::WindowManager::getInstance()->render();

    ImguiGlfwVulkanDebugger::render();
    ImGui_ImplVKGlfw_endRender();
  }

  void Application::shutdown() {
    ImGui_ImplVKGlfw_shutdown();

    // close the library
    dlclose(handle);
    if(plugin != nullptr){
      delete plugin;
    }
  }
}// namespace HummingBirdCore