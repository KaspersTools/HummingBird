//
// Created by Kasper de Bruin on 06/02/2024.
//

#include "../include/IPlugin.h"
#include <imgui.h>
#include <iostream>

class TestPlugin : public HummingBird::Plugins::IPlugin {
  public:
  void initialize() override {
    std::cout << "Hello, Plugin!" << std::endl;
  }
  void execute() override {
    std::cout << "Hello, Plugin execute!" << std::endl;
  }
  void cleanup() override {
    std::cout << "Hello, Plugin cleanup!" << std::endl;
  }
  void render(ImGuiContext* currentContext) override{
    ImGui::SetCurrentContext(currentContext);
    ImGui::Begin("TestPlugin");
    std::cout << "Hello, Plugin render!" << std::endl;
    ImGui::Text("Hello, Plugin render!");
    ImGui::Text("Hello, Plugin render1!");
    ImGui::Text("Hello, Plugin rende123r!");
    ImGui::Text("Hello, Plugin123 render!");
    ImGui::Text("Hello, Plug123in render!");
    ImGui::End();
  }
};

extern "C" HummingBird::Plugins::IPlugin* create_plugin() {
  return new TestPlugin();
}
