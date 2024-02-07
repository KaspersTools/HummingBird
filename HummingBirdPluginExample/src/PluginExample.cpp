//
// Created by Kasper de Bruin on 06/02/2024.
//

#include "PluginExample.h"

void PluginExample::initialize() {
  std::cout << "PluginManager initialized" << std::endl;
  HummingBirdCore::UI::WindowManager::getInstance()->addWindow("Plugin Example", 0, std::make_shared<PluginExampleWindow>());
}

void PluginExample::cleanup() {
  std::cout << "PluginManager cleaned up" << std::endl;
}

void PluginExample::update() {
}