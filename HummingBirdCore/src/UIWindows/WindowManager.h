//
// Created by Kasper de Bruin on 23/01/2024.
//

#pragma once

#include <UIWindows/UIWindow.h>

namespace HummingBirdCore::UI {
  class WindowManager {
public:
    WindowManager();

    ~WindowManager() = default;

    void addWindow(const std::string &name, const int addition, std::shared_ptr<UIWindow> uiWindow);
    void render();
    static WindowManager *getInstance();

private:
    // UIWindows
    std::map<std::string, std::shared_ptr<UIWindow>> m_windows;
  };
}// namespace HummingBirdCore::UI