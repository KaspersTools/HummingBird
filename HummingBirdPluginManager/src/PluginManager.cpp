//
// Created by Kasper de Bruin on 06/02/2024.
//

#include "PluginManager.h"

void PluginManager::initialize() {

}

void PluginManager::execute() {

}

void PluginManager::cleanup() {

}

void PluginManager::render(ImGuiContext* currentContext) {
  ImGui::SetCurrentContext(currentContext);
  ImGui::Begin("PluginManager");
  ImGui::Text("Hello, Plugin manager render!");
  ImGui::End();
}


extern "C" HummingBird::Plugins::IPlugin* create_plugin() {
  return new PluginManager();
}

