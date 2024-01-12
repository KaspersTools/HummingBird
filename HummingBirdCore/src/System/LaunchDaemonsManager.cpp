//
// Created by Kasper de Bruin on 08/01/2024.
//

#include "LaunchDaemonsManager.h"

namespace HummingBirdCore {
  namespace System {
    void LaunchDaemonsManager::render() {
      ImGui::BeginChild("LaunchDaemons", ImVec2(ImGui::GetContentRegionAvail().x * 0.2f, 0), c_leftChildFlags, c_leftWindowFlags);
      ImGui::Text("LaunchDaemons");
      ImGui::Separator();

      //tab
      if (ImGui::BeginTabBar("LaunchDaemonsTabBar")) {
        if (ImGui::BeginTabItem("User Agent")) {
          int index = 0;
          for (auto &daemon: m_userAgent) {
            if(ImGui::Selectable(daemon.getLabel().c_str(), m_selectedDaemon == index)){
              m_selectedDaemon = index;
            }
            index++;
          }
          ImGui::EndTabItem();
          ImGui::EndTabBar();
        }

        ImGui::EndChild();

        //Right resizeable panel
        ImGui::SameLine();
        ImGui::BeginGroup();

        ImGui::BeginChild("LaunchDaemonsManagerRightPanel", ImVec2(0, 0), true);

        ImGui::SameLine();

        {
          ImGui::BeginChild("SelectedDeamonRight", ImVec2(0, 0), true);
          ImGui::Text("Selected Daemon");
          ImGui::SameLine();
          if(ImGui::Button("Save")){
            m_userAgent[m_selectedDaemon].save();
          }
          ImGui::Separator();
          {
            ImGui::BeginChild("SelectedDaemonRightChild", ImVec2(0, 0), true);
            if(m_selectedDaemon >= m_userAgent.size()) {
              m_selectedDaemon = 0;
            }
            LaunchDaemon &daemon = m_userAgent[m_selectedDaemon];
            renderDaemon(daemon);
            ImGui::EndChild();
          }
          ImGui::EndChild();
        }
      }

      ImGui::EndChild();
      ImGui::EndGroup();
    }

    void LaunchDaemonsManager::renderDaemon(HummingBirdCore::System::LaunchDaemon &daemon) {
      Utils::PlistUtil::Plist *plist = daemon.getPlist();

      if (plist != nullptr) {
        int index = 0;
        for (auto &node: plist->getRootNode().children) {
          renderNode(node, index);
          index += 1;
        }
      }
    }

    void LaunchDaemonsManager::renderNode(HummingBirdCore::Utils::PlistUtil::PlistNode &node, int index) {
      //make a child that always fits the content

      std::string id = node.key + " - " + std::to_string(index);

      if (node.type == Utils::PlistUtil::PlistTypeString) {

        ImGui::InputText(id.c_str(), &std::get<std::string>(node.value));

      } else if (node.type == Utils::PlistUtil::PlistTypeInteger) {

        ImGui::InputInt(id.c_str(), &std::get<int>(node.value));

      } else if (node.type == Utils::PlistUtil::PlistTypeBoolean) {

        ImGui::Checkbox(id.c_str(), &std::get<bool>(node.value));

      } else if (node.type == Utils::PlistUtil::PlistTypeDate) {

        int items  = 5;
        float offset = 12;

        float sizeY = (ImGui::CalcTextSize("Weekday").y * items) + (offset*items);

        ImGui::BeginChild(id.c_str(), ImVec2(0, sizeY), true);
        {
          ImGui::PushID(id.c_str());
          ImGui::InputInt("Minute", &std::get<Utils::PlistUtil::PlistNode::Date>(node.value).minute);
          ImGui::InputInt("Hour", &std::get<Utils::PlistUtil::PlistNode::Date>(node.value).hour);
          ImGui::InputInt("Day", &std::get<Utils::PlistUtil::PlistNode::Date>(node.value).day);
          ImGui::InputInt("Month", &std::get<Utils::PlistUtil::PlistNode::Date>(node.value).month);
          ImGui::InputInt("Weekday", &std::get<Utils::PlistUtil::PlistNode::Date>(node.value).weekday);
          ImGui::PopID();
        }
        ImGui::EndChild();
      } else if (node.type == Utils::PlistUtil::PlistTypeArray) {
        ImGui::Text("Array");
        for (auto &childNode: node.children) {
          index++;
          renderNode(childNode, index);
        }
      } else if (node.type == Utils::PlistUtil::PlistTypeDictionary) {
        ImGui::Text("Dictionary");
        for (auto &childNode: node.children) {
          index++;
          renderNode(childNode, index);
        }
      }
    }

    void LaunchDaemonsManager::fetchAllDaemons() {
      std::vector<HummingBirdCore::Utils::File> files = HummingBirdCore::Utils::FolderUtils::getFilesInFolder(c_userAgentPath.string(), ".plist");

      for (const auto &file: files) {
        m_userAgent.emplace_back(file);
      }
    }
  }// namespace System
}// namespace HummingBirdCore