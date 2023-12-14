//
// Created by Kasper de Bruin on 11/12/2023.
//

#pragma once

#include <vendor/filedialog/FileBrowser/ImGuiFileBrowser.h>
#include <vendor/ImGuiColorTextEdit/TextEditor.h>

#include "../../UIWindows/UIWindow.h"
#include "../../Utils/Utils.h"

namespace HummingBirdCore::GeneralTools {
  class TextEditorWindow : public HummingBirdCore::UIWindow {
public:
    TextEditorWindow() : UIWindow(ImGuiWindowFlags_MenuBar) {}

    [[nodiscard]] TextEditor getTextEditor() const { return m_textEditor; }

    void render() override;

private:
    TextEditor m_textEditor;
    imgui_addons::ImGuiFileBrowser m_fileDialog;
  };

}// namespace HummingBirdCore::GeneralTools
