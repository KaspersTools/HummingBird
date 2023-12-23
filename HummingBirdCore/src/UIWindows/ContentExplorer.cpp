//
// Created by Kasper de Bruin on 17/12/2023.
//

#include "ContentExplorer.h"

namespace HummingBirdCore {
  namespace UIWindows {
    void HummingBirdCore::UIWindows::ContentExplorer::render() {

      if(ImGui::Button("<-")){
        if(m_backstack.size() > 0) {
          m_forwardstack.push_back(m_currentDirectory->getPath());
          m_currentDirectory = std::make_shared<ContentFolderItem>(m_backstack.back().string(),
                                                                   m_backstack.back().filename().string());
          m_backstack.pop_back();
          m_currentDirectory->setChildDirectories();
        }
      }
      ImGui::SameLine();
      if(ImGui::Button("->")){
        if(m_forwardstack.size() > 0) {
          m_backstack.push_back(m_currentDirectory->getPath());
          m_currentDirectory = std::make_shared<ContentFolderItem>(m_forwardstack.back().string(),
                                                                   m_forwardstack.back().filename().string());
          m_forwardstack.pop_back();
          m_currentDirectory->setChildDirectories();
        }
      }
      ImGui::SameLine();
      ImGui::SameLine();
      const char* fmt = "Path: %s" ;
      ImGui::Text(fmt, m_currentDirectory->getPath().c_str());
      ImGui::Separator();

      ImGui::BeginChild("###ContentExplorerChild", ImVec2(0, 0), true);
      for(auto &item : m_currentDirectory->getSubDirectories()) {
        if (ImGui::Selectable(item.getName().c_str())){
          changeDirectory(item);
        }
      }
      ImGui::EndChild();

    }
  }// namespace UIWindows
}// namespace HummingBirdCore