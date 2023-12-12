//
// Created by Kasper de Bruin on 11/12/2023.
//

#include "TextEditorWindow.h"

namespace HummingBird {
  namespace GeneralTools {

    void TextEditorWindow::Render() {
      bool open = false, save = false;
      if(ImGui::BeginMenuBar())
      {
        if (ImGui::BeginMenu("Menu"))
        {
          if (ImGui::MenuItem("Open", NULL))
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
//        std::cout << file_dialog.selected_fn << std::endl;  // The name of the selected file or directory in case of Select Directory dialog mode
//        std::cout << file_dialog.selected_path << std::endl;// The absolute path to the selected file

        std::string fileContent = FileUtils::readFromFile(file_dialog.selected_path);
                m_textEditor.SetText(fileContent);
      }


      m_textEditor.Render("TextEditor");
    }



  }// namespace GeneralTools
}// namespace HummingBird