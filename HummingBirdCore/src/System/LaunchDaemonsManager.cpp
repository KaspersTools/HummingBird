//
// Created by Kasper de Bruin on 08/01/2024.
//

#include "LaunchDaemonsManager.h"

namespace HummingBirdCore {
  namespace System {

    void LaunchDaemonsManager::renderNode(Utils::PlistUtil::PlistNode &node) {
      std::string id;
      if (node.key.empty()) {
        id = "##Unknown";
        ImGui::Text("Unknown");
      } else {
        id = "##" + node.key;
        ImGui::Text(node.key.c_str());
      }
      if (node.type == Utils::PlistUtil::PlistTypeString) {
        std::string val = std::get<std::string>(node.value);
        if (ImGui::InputText(id.c_str(), &val)) {
          node.value = val;
        }
      } else if (node.type == Utils::PlistUtil::PlistTypeInteger) {
        int val = std::get<int>(node.value);
        if (ImGui::DragInt(id.c_str(), &val)) {
          node.value = val;
        }
      } else if (node.type == Utils::PlistUtil::PlistTypeReal) {
        float val = std::get<float>(node.value);
        if (ImGui::DragFloat(id.c_str(), &val)) {
          node.value = val;
        }
      } else if (node.type == Utils::PlistUtil::PlistTypeBoolean) {
        bool val = std::get<bool>(node.value);
        if (ImGui::Checkbox(id.c_str(), &val)) {
          node.value = val;
        }
      } else if (node.type == Utils::PlistUtil::PlistTypeDate) {
        Utils::PlistUtil::PlistNode::Date val = std::get<Utils::PlistUtil::PlistNode::Date>(node.value);
        if (ImGui::DragInt("Month", &val.month)) {
          node.value = val;
        }
        ImGui::SameLine();
        if (ImGui::DragInt("Day", &val.day)) {
          node.value = val;
        }
        ImGui::SameLine();
        if (ImGui::DragInt("Hour", &val.hour)) {
          node.value = val;
        }
        ImGui::SameLine();
        if (ImGui::DragInt("Minute", &val.minute)) {
          node.value = val;
        }
        ImGui::SameLine();
        if (ImGui::DragInt("Weekday", &val.weekday)) {
          node.value = val;
        }
      } else if (node.type == Utils::PlistUtil::PlistTypeData) {
        ImGui::Text("Data");
      } else if (node.type == Utils::PlistUtil::PlistTypeArray) {
        for (auto &child: node.children) {
          renderNode(child);
        }
      } else if (node.type == Utils::PlistUtil::PlistTypeDictionary) {
        for (auto &child: node.children) {
          renderNode(child);
        }
      } else if (node.type == Utils::PlistUtil::PlistTypeNone) {
        ImGui::Text("Unknown");
      }
    }
    void LaunchDaemonsManager::render() {
      ImGui::BeginChild("LaunchDaemons", ImVec2(ImGui::GetContentRegionAvail().x * 0.2f, 0), c_leftChildFlags, c_leftWindowFlags);
      ImGui::Text("LaunchDaemons");
      ImGui::Separator();

      //tab
      if (ImGui::BeginTabBar("LaunchDaemonsTabBar")) {
        if (ImGui::BeginTabItem("User Agent")) {
          renderDaemonsTable(m_userAgent);
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
          ImGui::Separator();
          {
            ImGui::BeginChild("SelectedDaemonRightChild", ImVec2(0, 0), true);
            if (m_selectedDaemon != nullptr) {
              const Utils::PlistUtil::Plist pl = m_selectedDaemon->getPlist();
              if (pl.parsed && pl.hasChildren()) {
                for (auto &child: pl.getChildren()) {
                  renderNode(child);
                }
              }
            }
            ImGui::EndChild();
          }
          ImGui::EndChild();
        }
      }

      ImGui::EndChild();
      ImGui::EndGroup();
    }

    void LaunchDaemonsManager::fetchAllDaemons() {

      std::vector<HummingBirdCore::Utils::File> files = HummingBirdCore::Utils::FolderUtils::getFilesInFolder(c_userAgentPath.string(), ".plist");

      for (const auto &file: files) {
        LaunchDaemon *da = new LaunchDaemon(file);
        m_userAgent.push_back(da);
      }
    }

    void LaunchDaemonsManager::renderDaemonsTable(std::vector<LaunchDaemon *> daemons) {
      ImGui::BeginTable("LaunchDaemonsTable", 1, ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders);

      ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed);

      ImGui::TableSetupScrollFreeze(0, 1);// Make row(header) always visible
      ImGui::TableHeadersRow();

      for (LaunchDaemon *daemon: daemons) {
        ImGui::PushID(daemon);
        ImGui::TableNextRow();
        {
          ImGui::TableNextColumn();
          if (ImGui::Selectable(daemon->getFile().name.c_str(), m_selectedDaemon == daemon)) {
            selectDaemon(daemon);
          }
        }
        ImGui::PopID();
      }
      ImGui::EndTable();
    }

    void LaunchDaemonsManager::selectDaemon(LaunchDaemon *daemon) {
      //TODO: Check If current is unsaved else show modal
      CORE_TRACE("Selected daemon: " + daemon->getFile().name);
      m_selectedDaemon = daemon;
      m_selectedDaemon->init();
    }
  }// namespace System
}// namespace HummingBirdCore