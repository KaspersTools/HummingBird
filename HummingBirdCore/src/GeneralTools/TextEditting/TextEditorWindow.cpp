//
// Created by Kasper de Bruin on 11/12/2023.
//

#include "TextEditorWindow.h"

namespace HummingBirdCore {
  namespace GeneralTools {

    void TextEditorWindow::Render() {
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

      //Remember the name to ImGui::OpenPopup() and showFileDialog() must be same...
      if(open)
        ImGui::OpenPopup("Open File");

      /* Optional third parameter. Support opening only compressed rar/zip files.
     * Opening any other file will show error, return false and won't close the dialog.
     */
      if (file_dialog.showFileDialog("Open File", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310))) {
        //TODO: recreate this functionality
//        std::string fileContent = FileUtils::readFromFile(file_dialog.selected_path, false);
//                m_textEditor.SetText(fileContent);
      }


      m_textEditor.Render("TextEditor");
    }



  }// namespace GeneralTools
}// namespace  HummingBirdCore