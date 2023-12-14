//
// Created by Kasper de Bruin on 01/11/2023.
//

#pragma once

#include <ImGui.h>

// Define a UI window structure
namespace HummingBirdCore {
  class UIWindow {
public:
    UIWindow(ImGuiWindowFlags flags) : m_flags(flags) {}
    virtual void render() = 0;// Pure virtual function to be implemented by derived classes
    bool isOpen() const { return m_isOpen; }

    ImGuiWindowFlags getFlags() const { return m_flags; }
    bool m_isOpen = true;// Control the visibility of the window

public:
    ImGuiWindowFlags m_flags;
  };
}// namespace HummingBirdCore
