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
            if (ImGui::Selectable(daemon.getLabel().c_str(), m_selectedDaemon == index)) {
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
          if (ImGui::Button("Save")) {
            m_userAgent[m_selectedDaemon].save();
          }
          ImGui::SameLine();

          if(ImGui::Button("New"))
          {
            Utils::File f = Utils::File("NewDaemon", c_userAgentPath, ".plist", "");
            m_userAgent.emplace_back(f);
          }
          ImGui::Separator();
          {
            ImGui::BeginChild("SelectedDaemonRightChild", ImVec2(0, 0), true);
            if (m_selectedDaemon >= m_userAgent.size()) {
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
        if (ImGui::Button("Add calendar interval")) {
          plist->addCalendarIntervalToRootNode();
        }
        ImGui::Separator();

        for (auto &node: plist->getRootNode().children) {
          renderNode(node, index);
          index += 1;
        }
      }
    }

    void LaunchDaemonsManager::renderNode(HummingBirdCore::Utils::PlistUtil::PlistNode &node, int index) {
      std::string id = node.key + " - " + std::to_string(index);

      if (node.type == Utils::PlistUtil::PlistTypeString) {
        std::string val = std::get<std::string>(node.getValue());
        int colorCount = 0;
        if (node.required && !node.value.has_value()) {
          ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
          colorCount++;
        }

        if (ImGui::InputText(id.c_str(), &val)) {
          node.setValue(val);
        }

        if (colorCount > 0) {
          ImGui::PopStyleColor(colorCount);
        }
      } else if (node.type == Utils::PlistUtil::PlistTypeInteger) {
        int colorCount = 0;
        int val = std::get<int>(node.getValue());
        if (node.required && !node.value.has_value()) {
          ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
          colorCount++;
        }
        if (ImGui::InputInt(id.c_str(), &val)) {
          node.setValue(val);
        }

        if (colorCount > 0) {
          ImGui::PopStyleColor(colorCount);
        }
      } else if (node.type == Utils::PlistUtil::PlistTypeBoolean) {
        int colorCount = 0;
        bool val = std::get<bool>(node.getValue());
        if (node.required && !node.value.has_value()) {
          ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
          colorCount++;
        }
        if (ImGui::Checkbox(id.c_str(), &val)) {
          node.setValue(val);
        }

        if (colorCount > 0) {
          ImGui::PopStyleColor(colorCount);
        }
      } else if (node.type == Utils::PlistUtil::PlistTypeArray) {
        int colorCount = 0;
        if (node.required && !node.value.has_value()) {
          ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
          colorCount++;
        }
        if (ImGui::TreeNode(id.c_str())) {
          int index = 0;
          for (auto &child: node.children) {
            renderNode(child, index);
            index++;
          }
          ImGui::TreePop();
        }

        if (colorCount > 0) {
          ImGui::PopStyleColor(colorCount);
        }
      } else if (node.type == Utils::PlistUtil::PlistTypeDictionary) {
        int colorCount = 0;
        if (node.required && !node.value.has_value()) {
          ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
          colorCount++;
        }
        if (ImGui::TreeNode(id.c_str())) {
          int index = 0;
          for (auto &child: node.children) {
            renderNode(child, index);
            index++;
          }
          ImGui::TreePop();
        }
      } else if (node.type == Utils::PlistUtil::PlistTypeDate) {
        int colorCount = 0;
        Utils::PlistUtil::PlistNode::Date &date = std::get<Utils::PlistUtil::PlistTypeDate>(node.getValue());
        if (node.required && !node.value.has_value()) {
          ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
          colorCount++;
        }
        if (ImGui::TreeNode(id.c_str())) {
          if (ImGui::InputInt("Weekday: ", &date.weekday)) {
            node.setValue(date);
          }
          if (ImGui::InputInt("Month: ", &date.month)) {
            node.setValue(date);
          }
          if (ImGui::InputInt("Day: ", &date.day)) {
            node.setValue(date);
          }
          if (ImGui::InputInt("Hour: ", &date.hour)) {
            node.setValue(date);
          }
          if (ImGui::InputInt("Minute: ", &date.minute)) {
            node.setValue(date);
          }
          ImGui::TreePop();
        }

        if (colorCount > 0) {
          ImGui::PopStyleColor(colorCount);
        }
      }

      if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip(node.getDescription().c_str());
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