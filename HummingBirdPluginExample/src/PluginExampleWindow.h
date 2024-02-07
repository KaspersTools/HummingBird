//
// Created by Kasper de Bruin on 07/02/2024.
//

#ifndef HUMMINGBIRD_PLUGINMANAGERWINDOW_H
#define HUMMINGBIRD_PLUGINMANAGERWINDOW_H

#include <HBUI/HBUI.h>
#include <HBUI/UIWindow.h>
#include <HBUI/WindowManager.h>

class PluginExampleWindow : public HummingBirdCore::UIWindow {
  public:
  PluginExampleWindow() : UIWindow("Plugin Example") {
  }

  void render() override {
    ImGui::Text("This is the plugin exsmple window");
    ImGui::InputText("Plugin Name", &inputTest);
    ImGui::Button("test button");
  }

  private:
  std::string inputTest = "test";
};


#endif//HUMMINGBIRD_PLUGINMANAGERWINDOW_H
