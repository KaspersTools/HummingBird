//
// Created by Kasper de Bruin on 01/11/2023.
//

#ifndef KBTOOLS_UIWINDOW_H
#define KBTOOLS_UIWINDOW_H

#include <ImGui.h>

// Define a UI window structure
namespace KBTools {
    class UIWindow {
    public:
        UIWindow(const ImGuiWindowFlags flags) : m_flags(flags) {}
        virtual void Render() = 0;  // Pure virtual function to be implemented by derived classes
        const bool IsOpen() const { return isOpen; }

        bool isOpen = true;  // Control the visibility of the window
        const ImGuiWindowFlags GetFlags() const { return m_flags; }
    public:
      ImGuiWindowFlags m_flags;
    };
}

#endif //KBTOOLS_UIWINDOW_H
