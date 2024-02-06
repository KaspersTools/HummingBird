//
// Created by Kasper de Bruin on 06/02/2024.
//

#ifndef HUMMINGBIRD_PLUGINMANAGER_H
#define HUMMINGBIRD_PLUGINMANAGER_H
#include "../include/IPlugin.h"
#include "HBUI/HBUI.h"

class PluginManager : public HummingBird::Plugins::IPlugin {
  public:
  void initialize() override;
  void execute() override;
  void cleanup() override;
  void render(ImGuiContext* currentContext) override;
};

#endif//HUMMINGBIRD_PLUGINMANAGER_H
