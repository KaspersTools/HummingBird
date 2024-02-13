//
// Created by Kasper de Bruin on 07/02/2024.
//

#ifndef HUMMINGBIRD_PLUGINMANAGERWINDOW_H
#define HUMMINGBIRD_PLUGINMANAGERWINDOW_H

#include <HBUI/HBUI.h>
#include <HBUI/UIWindow.h>
#include <HBUI/WindowManager.h>
#include <filesystem>


class PluginManagerWindow : public HummingBirdCore::UIWindow {
  public:
  PluginManagerWindow() : UIWindow("Plugin Manager") {
  }

  void render() override {
    if (ImGui::BeginTabBar("Plugin Manager")) {
      if (ImGui::BeginTabItem("Updates")) {
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "not implemented.");
        ImGui::EndTabItem();
      }
      if (ImGui::BeginTabItem("Available")) {
        renderAvailablePlugins();
        ImGui::EndTabItem();
      }
      if (ImGui::BeginTabItem("Loaded")) {
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "not implemented.");
        ImGui::EndTabItem();
      }
      if (ImGui::BeginTabItem("Settings")) {
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "not implemented.");
        ImGui::EndTabItem();
      }
      ImGui::EndTabBar();
    }
  }

  void renderAvailablePlugins() {
    std::filesystem::path path = "plugins/";
    path = absolute(path);
    if (!std::filesystem::exists(path)) {
      const std::string pathStr = path.string();
      const std::string error = "Path does not exist: " + pathStr;
      ImGui::TextColored(ImVec4(1, 0, 0, 1), error.c_str());
      return;
    }
    for (const auto &entry: std::filesystem::directory_iterator(path)) {
      if (entry.is_regular_file()) {
        const std::string filename = entry.path().filename().string();
        ImGui::Text(filename.c_str());
      }
    }
  }

  private:
  std::string inputTest = "test";
};


#endif//HUMMINGBIRD_PLUGINMANAGERWINDOW_H
