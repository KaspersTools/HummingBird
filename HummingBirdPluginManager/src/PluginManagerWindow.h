//
// Created by Kasper de Bruin on 07/02/2024.
//

#ifndef HUMMINGBIRD_PLUGINMANAGERWINDOW_H
#define HUMMINGBIRD_PLUGINMANAGERWINDOW_H

#include <HBUI/HBUI.h>
#include <HBUI/UIWindow.h>
#include <HBUI/WindowManager.h>

class PluginManagerWindow : public HummingBirdCore::UIWindow {
public:
    PluginManagerWindow() : UIWindow("Plugin Manager") {

    }

    void render() override {
        ImGui::Text("This is the plugin manager window");
        ImGui::InputText("Plugin Name", &inputTest);
    }

private:
    std::string inputTest = "test";
};


#endif //HUMMINGBIRD_PLUGINMANAGERWINDOW_H
