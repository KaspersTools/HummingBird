//
// Created by Kasper de Bruin on 06/02/2024.
//

#include "PluginManager.h"

#include <HBUI/HBUI.h>

void PluginManager::initialize() {
  std::cout << "PluginManager initialized" << std::endl;
}

void PluginManager::execute() {
  std::cout << "PluginManager executed" << std::endl;
}

void PluginManager::cleanup() {
  std::cout << "PluginManager cleaned up" << std::endl;
}

void PluginManager::render() {
  ImGui::Begin("PluginManager");
  ImGui::Text("PluginManager");
  ImGui::End();
}