//
// Created by Kasper de Bruin on 01/11/2023.
//

#pragma once

#include "ApplicationObject.h"

#include <ImGui.h>

// Define a UI window structure
namespace HummingBirdCore {
  class UIWindow : public ApplicationObject{
public:
    UIWindow(ImGuiWindowFlags flags, const std::string& name) : m_flags(flags), c_name(name){}
    virtual void render() = 0;// Pure virtual function to be implemented by derived classes

    std::string getName() const { return c_name; }
    bool isOpen() const { return m_isOpen; }

    ImGuiWindowFlags getFlags() const { return m_flags; }


public:
    bool m_isOpen = true;// Control the visibility of the window

public:
    ImGuiWindowFlags m_flags;
    const std::string c_name;
  };
}// namespace HummingBirdCore
