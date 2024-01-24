//
// Created by Kasper de Bruin on 23/01/2024.
//

#include <UIWindows/WindowManager.h>

namespace HummingBirdCore::UI {
  WindowManager *s_instance = nullptr;

  WindowManager::WindowManager() {
    s_instance = this;
  }

  WindowManager *WindowManager::getInstance() {
    if (s_instance == nullptr) {
      s_instance = new WindowManager();
    }

    return s_instance;
  }

  void WindowManager::addWindow(const std::string &baseName, const int addition, std::shared_ptr<UIWindow> uiWindow) {
    const std::string name = baseName + std::to_string(addition);
    if (m_windows.find(name) == m_windows.end()) {
      m_windows.insert(std::pair<std::string, std::shared_ptr<UIWindow>>(name , uiWindow));
      uiWindow->setName(name);
    } else {
      addWindow(baseName, addition + 1, uiWindow);
    }
  }

  void WindowManager::render(){
    for (auto &window : m_windows) {
      window.second->beginFrame();
    }
  }
}// namespace HummingBirdCore::UI