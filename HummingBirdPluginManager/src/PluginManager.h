//
// Created by Kasper de Bruin on 06/02/2024.
//

#ifndef HUMMINGBIRD_PLUGINMANAGER_H
#define HUMMINGBIRD_PLUGINMANAGER_H
#include "../include/IPlugin.h"
#include <HBUI/HBUI.h>

#include "PluginManagerWindow.h"
#include <filesystem>
#include <iostream>
#include <vector>

#include <dlfcn.h>
#include <iostream>

struct PluginData{
  std::string name;
  std::filesystem::path fullPath;
};

class PluginManager : public HummingBird::Plugins::IPlugin {
  public:
  PluginManager(HummingBirdCore::UI::WindowManager *windowManagerPtr, ImGuiContext *imGuiContext,
                ImGuiMemAllocFunc allocFunc, ImGuiMemFreeFunc freeFunc, void *userData) : IPlugin(windowManagerPtr, imGuiContext, allocFunc, freeFunc, userData) {
  }
  void initialize() override;
  void update() override;
  void cleanup() override;

  bool addPlugin(const std::filesystem::path &path) {
    // Pass the context to the libraries
    void *handle = dlopen(path.string().c_str(),
                          RTLD_LAZY);
    if (!handle) {
      std::string err = dlerror();
      std::string error = "cannot open library: " + path.string() + " " + err;
      std::cerr << error << std::endl;
      return false;
    }
    handles.push_back(handle);

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
      std::cerr << error << std::endl;
      dlclose(handle);
      return false;
    }
    ImGuiMemAllocFunc p_alloc;
    ImGuiMemFreeFunc p_free;
    void *p_user_data;
    ImGui::GetAllocatorFunctions(&p_alloc, &p_free, &p_user_data);
    HummingBird::Plugins::IPlugin *plugin = create_plugin(
            HummingBirdCore::UI::WindowManager::getInstance(), ImGui::GetCurrentContext(), p_alloc, p_free, p_user_data);

    if (plugin == nullptr) {
      std::cerr << "Failed to create plugin manager" << std::endl;
      dlclose(handle);
      return false;
    }

    plugins.push_back(plugin);
    plugin->initialize();
    return true;
  }

  private:
  std::vector<HummingBird::Plugins::IPlugin *> plugins = {};
  std::vector<void *> handles = {};
};

extern "C" HummingBird::Plugins::IPlugin *create_plugin(
        HummingBirdCore::UI::WindowManager *windowManagerPtr, ImGuiContext *imGuiContext,
        ImGuiMemAllocFunc allocFunc, ImGuiMemFreeFunc freeFunc, void *userData) {
  return new PluginManager(windowManagerPtr, imGuiContext, allocFunc, freeFunc, userData);
}

extern "C" bool loadPlugin(const std::filesystem::path &path, HummingBird::Plugins::IPlugin *pluginManager) {
  return static_cast<PluginManager *>(pluginManager)->addPlugin(path);
}

#endif//HUMMINGBIRD_PLUGINMANAGER_H
