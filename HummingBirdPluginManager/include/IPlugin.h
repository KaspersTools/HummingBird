//
// Created by Kasper de Bruin on 06/02/2024.
//

#ifndef HUMMINGBIRD_PLUGIN_MANAGER_IPLUGIN_H
#define HUMMINGBIRD_PLUGIN_MANAGER_IPLUGIN_H
#include <imgui.h>

namespace HummingBird::Plugins {
  class IPlugin {
public:
    virtual void initialize() = 0;
    virtual void execute() = 0;
    virtual void render(ImGuiContext* currentContext) =0;
    virtual void cleanup() = 0;
  };
}// namespace HummingBird::PluginManager
#endif//HUMMINGBIRD_PLUGIN_MANAGER_IPLUGIN_H
