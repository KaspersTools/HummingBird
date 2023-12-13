//
// Created by Kasper de Bruin on 11/12/2023.
//

#pragma once

#include <vendor/filedialog/FileBrowser/ImGuiFileBrowser.h>

namespace HummingBirdCore::GeneralTools {
//Everything in this class is static
  class FileDialog {
public:
    inline static void Open(const bool isOpen = true){
      s_isOpen = isOpen;
    }

    inline static void Render() {

      //Remember the name to ImGui::OpenPopup() and showFileDialog() must be same...
      if(s_isOpen)
        ImGui::OpenPopup("Open File");

      /* Optional third parameter. Support opening only compressed rar/zip files.
     * Opening any other file will show error, return false and won't close the dialog.
     */
      if(s_fileDialog.showFileDialog("Open File", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310), ".rar,.zip,.7z"))
      {
//        std::cout << s_fileDialog.selected_fn << std::endl;      // The name of the selected file or directory in case of Select Directory dialog mode
//        std::cout << s_fileDialog.selected_path << std::endl;    // The absolute path to the selected file
      }
    }



private:
    inline static imgui_addons::ImGuiFileBrowser s_fileDialog;
    inline static bool s_isOpen = false;
  };
}// namespace HummingBirdCore::GeneralTools

