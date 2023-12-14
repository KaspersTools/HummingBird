//
// Created by Kasper de Bruin on 11/12/2023.
//

#include "TextEditorWindow.h"

namespace HummingBirdCore {
  namespace GeneralTools {

    void TextEditorWindow::render() {
      bool open = false;
      if(ImGui::BeginMenuBar())
      {
        if (ImGui::BeginMenu("Menu"))
        {
          if (ImGui::MenuItem("Open"))
            open = true;
          ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
      }

      if(open)
        ImGui::OpenPopup("Open File");

      if (m_fileDialog.showFileDialog("Open File", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310))) {
        //TODO: recreate this functionality
      }


      m_textEditor.Render("TextEditor");
    }



  }// namespace GeneralTools
}// namespace  HummingBirdCore