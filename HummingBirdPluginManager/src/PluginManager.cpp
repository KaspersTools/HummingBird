//
// Created by Kasper de Bruin on 06/02/2024.
//

#include "PluginManager.h"

void PluginManager::initialize() {
  std::cout << "PluginManager initialized" << std::endl;
  HummingBirdCore::UI::WindowManager::getInstance()->addWindow("PluginManager", 0, std::make_shared<PluginManagerWindow>());
}

void PluginManager::cleanup() {
  std::cout << "PluginManager cleaned up" << std::endl;
  for (auto &plugin : plugins) {
    plugin->cleanup();
  }
}

void PluginManager::update() {
  for (auto &plugin : plugins) {
    plugin->update();
  }
}