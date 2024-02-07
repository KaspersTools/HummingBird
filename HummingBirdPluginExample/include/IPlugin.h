//
// Created by Kasper de Bruin on 06/02/2024.
//

#ifndef HUMMINGBIRD_PLUGIN_MANAGER_IPLUGIN_H
#define HUMMINGBIRD_PLUGIN_MANAGER_IPLUGIN_H
#include <HBUI/HBUI.h>
#include <HBUI/WindowManager.h>

namespace HummingBird::Plugins {
  class IPlugin {
public:
      IPlugin(HummingBirdCore::UI::WindowManager *windowManagerPtr, ImGuiContext *imGuiContext,
            ImGuiMemAllocFunc allocFunc, ImGuiMemFreeFunc freeFunc, void *userData) {
      HummingBirdCore::UI::WindowManager::setInstance(windowManagerPtr);
      ImGui::SetCurrentContext(imGuiContext);
      ImGui::SetAllocatorFunctions(allocFunc, freeFunc, userData);
    }

    virtual void initialize() = 0;
    virtual void update() = 0;
    virtual void cleanup() = 0;
  };
}// namespace HummingBird::Plugins
#endif//HUMMINGBIRD_PLUGIN_MANAGER_IPLUGIN_H
