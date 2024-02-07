//
// Created by Kasper de Bruin on 01/11/2023.
//


#include <imconfig.h>
#include <misc/cpp/imgui_stdlib.h>

#include <addons/imguinotify/imgui_notify.h>
#include <imgui.h>
#include <imgui_internal.h>

#include "Application.h"
#include <KDB_ImGui/backends/debug/imgui_impl_glfw_vulkan_debug.h>
#include <KDB_ImGui/backends/imgui_impl_glfw_vulkan_window.h>

#include <KDB_ImGui/fonts/FontManager.h>

#include <HBUI/WindowManager.h>
#include <UIWindows/MainMenuBar.h>

#include <portable-file-dialogs.h>

#include <dlfcn.h>
#include <iostream>

typedef bool (*LoadPluginFunc)(const std::filesystem::path &path, HummingBird::Plugins::IPlugin *pluginManager);

namespace HummingBirdCore {
  void Application::init() {
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
      std::cout << "GLFWVULKANIMPL::errorCallback() error: " << error << " description: " << description
                << std::endl;
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
    KDB_IMGUI::FontManager::addFont("Assets/Fonts/JetBrainsMono/JetBrainsMonoNerdFontPropo-Regular.ttf",
                                    "JetBrainsMono", 16.0f, fontConfig);

    HummingBirdCore::UI::WindowManager *windowManager = new UI::WindowManager();
    HummingBirdCore::UI::WindowManager::setInstance(windowManager);

    if (!loadPluginManager("plugins/libHUMMINGBIRD_PLUGIN_MANAGER.dylib")) {
      CORE_ERROR("Failed to load plugin manager");
    }
    run();
  }

  bool Application::loadPluginManager(const std::filesystem::path &path) {
    // Pass the context to the libraries
    handle = dlopen(path.string().c_str(),
                    RTLD_LAZY);
    if (!handle) {
      std::string err = dlerror();
      std::string error = "cannot open library: " + path.string() + " " + err;
      CORE_ERROR(error);
      return false;
    }

    // reset errors
    dlerror();

    // load the symbols
    HummingBird::Plugins::IPlugin *(*create_plugin)(HummingBirdCore::UI::WindowManager *windowManagerPtr, ImGuiContext *imGuiContext,
                                                    ImGuiMemAllocFunc allocFunc, ImGuiMemFreeFunc freeFunc, void *userData);
    *(void **) (&create_plugin) = dlsym(handle, "create_plugin");

    const char *dlsym_error = dlerror();
    if (dlsym_error) {
      std::string err = dlsym_error;
      std::string error = "cannot load symbol create_plugin " + err;
      CORE_ERROR(error);
      dlclose(handle);
      return false;
    }
    ImGuiMemAllocFunc p_alloc;
    ImGuiMemFreeFunc p_free;
    void *p_user_data;
    ImGui::GetAllocatorFunctions(&p_alloc, &p_free, &p_user_data);
    pluginManager = create_plugin(HummingBirdCore::UI::WindowManager::getInstance(), ImGui::GetCurrentContext(), p_alloc, p_free, p_user_data);
    if (pluginManager == nullptr) {
      CORE_ERROR("Failed to create plugin manager");
      dlclose(handle);
      return false;
    }

    pluginManager->initialize();
    LoadPluginFunc loadPlugin = (LoadPluginFunc) dlsym(handle, "loadPlugin");

    if (loadPlugin == nullptr) {
      CORE_ERROR("Failed to load symbol loadPlugin");
      dlclose(handle);
      pluginManager->cleanup();
      delete pluginManager;
      return false;
    }

    loadPlugin("plugins/libHUMMINGBIRD_PLUGIN_TEMPLATE.dylib", pluginManager);

    return true;
  }

  bool Application::run() {
    while (!ImGui_ImplVKGlfw_shouldClose()) {
      if(pluginManager)
        pluginManager->update();
      render();
    }
  }

  void Application::render() {
    ImGui_ImplVKGlfw_startRender();

    ImGui::ShowDemoWindow();

    HummingBirdCore::UI::WindowManager::getInstance()->render();

    ImguiGlfwVulkanDebugger::render();
    ImGui_ImplVKGlfw_endRender();
  }

  void Application::shutdown() {
    CORE_INFO("Shutting down");
    if (pluginManager != nullptr) {
      pluginManager->cleanup();
      delete pluginManager;
    }
    dlclose(handle);
    ImGui_ImplVKGlfw_shutdown();

  }
}// namespace HummingBirdCore