//
// Created by Kasper de Bruin on 06/02/2024.
//

#ifndef HUMMINGBIRD_PLUGINMANAGER_H
#define HUMMINGBIRD_PLUGINMANAGER_H
#include "../include/IPlugin.h"
#include <HBUI/HBUI.h>

#include "PluginExampleWindow.h"

class PluginExample : public HummingBird::Plugins::IPlugin {
  public:
  PluginExample(HummingBirdCore::UI::WindowManager *windowManagerPtr, ImGuiContext *imGuiContext,
                ImGuiMemAllocFunc allocFunc, ImGuiMemFreeFunc freeFunc, void *userData) : IPlugin(windowManagerPtr, imGuiContext, allocFunc, freeFunc, userData) {
  }
  void initialize() override;
  void update() override;
  void cleanup() override;
};

extern "C" HummingBird::Plugins::IPlugin *create_plugin(
        HummingBirdCore::UI::WindowManager *windowManagerPtr, ImGuiContext *imGuiContext,
        ImGuiMemAllocFunc allocFunc, ImGuiMemFreeFunc freeFunc, void *userData) {
  return new PluginExample(windowManagerPtr, imGuiContext, allocFunc, freeFunc, userData);
}


#endif//HUMMINGBIRD_PLUGINMANAGER_H
