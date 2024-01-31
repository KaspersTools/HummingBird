//
// Created by Kasper de Bruin on 06/12/2023.
//

#pragma once
#include <PCH/pch.h>

namespace HummingBirdCore::Security {

  class LogInWindow : public UIWindow {
public:
    LogInWindow() : LogInWindow(ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings |
                                ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
                                ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse) {}

    LogInWindow(const ImGuiWindowFlags flags) : UIWindow("Log In", flags) {
    }


    ~LogInWindow();

    void render() override;

private:
    char m_username[32] = "admin";
    char m_password[32] = "admin";
    bool m_remember;
    bool m_failedLogin = false;
  };

}// namespace HummingBirdCore::Security
