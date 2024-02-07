//
// Created by Kasper de Bruin on 06/02/2024.
//

#ifndef HUMMINGBIRD_PLUGINMANAGER_H
#define HUMMINGBIRD_PLUGINMANAGER_H
#include "../include/IPlugin.h"
#include <HBUI/HBUI.h>

class PluginManager : public HummingBird::Plugins::IPlugin {
  public:
  void initialize() override;
  void execute() override;
  void render() override;
  void cleanup() override;
};

extern "C" HummingBird::Plugins::IPlugin* create_plugin() {
  return new PluginManager();
}

extern "C" void setImGuiContext(ImGuiContext* context) {
  ImGui::SetCurrentContext(context);
}
#endif//HUMMINGBIRD_PLUGINMANAGER_H
