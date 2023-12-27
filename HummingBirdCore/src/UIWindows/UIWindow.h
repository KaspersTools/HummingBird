//
// Created by Kasper de Bruin on 01/11/2023.
//

#pragma once

#include "ApplicationObject.h"

#include <imgui.h>

// Define a UI window structure
namespace HummingBirdCore {
  class UIWindow : public ApplicationObject{
public:
    UIWindow(ImGuiWindowFlags flags, const std::string& name, const bool autoEndFrame = true) :
                                                                                                 m_flags(flags), c_name(name), c_autoEndFrame(autoEndFrame){}
    virtual void render() = 0;// Pure virtual function to be implemented by derived classes

    std::string getName() const { return c_name; }
    bool isOpen() const { return m_isOpen; }

    ImGuiWindowFlags getFlags() const { return m_flags; }

    bool getAutoEndFrame() const { return c_autoEndFrame; }


public:
    bool m_isOpen = true;// Control the visibility of the window
    const std::string c_name;
    ImGuiWindowFlags m_flags;
private:
    const bool c_autoEndFrame;// Control if the window should end the frame
  };
}// namespace HummingBirdCore
