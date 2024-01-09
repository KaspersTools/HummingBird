//
// Created by Kasper de Bruin on 08/01/2024.
//

#include "LaunchDaemonsManager.h"

namespace HummingBirdCore {
  namespace System {
    void LaunchDaemonsManager::render() {
      bool changedVal = false;
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

        if (m_selectedDaemon != nullptr) {
          ImGui::BeginChild("SelectedDeamonLeftXML", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 0), c_leftChildFlags, c_leftWindowFlags);
          ImGui::Text("Selected Daemon");
          //TODO: add xml highligting
          ImGui::Checkbox("Wrap Text", &m_wrapText);
          ImGui::Separator();
          {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyle().Colors[ImGuiCol_WindowBg]);
            ImGui::BeginChild("SelectedDaemonLeftXMLChild", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysUseWindowPadding);

            if (m_wrapText) {
              ImGui::TextWrapped(m_selectedDaemon->getFile().content.c_str());
            } else {
              ImGui::Text(m_selectedDaemon->getFile().content.c_str());
            }

            ImGui::EndChild();
            ImGui::PopStyleColor();
            ImGui::PopStyleVar(5);
          }

          ImGui::EndChild();
        }

        ImGui::SameLine();

        {
          ImGui::BeginChild("SelectedDeamonRight", ImVec2(0, 0), true);
          ImGui::Text("Selected Daemon");
          ImGui::Separator();
          {
            ImGui::BeginChild("SelectedDaemonRightChild", ImVec2(0, 0), true);
            if (m_selectedDaemon != nullptr) {

              for (const auto &set_values: m_selectedDaemon->m_values) {
                std::string key = set_values.first;
                std::string value = set_values.second.value;
                std::string type = set_values.second.type;
              }
            }
            ImGui::EndChild();
          }
          ImGui::EndChild();
        }
      }

      ImGui::EndChild();
      ImGui::EndGroup();

//      if(m_selectedDaemon->getEnabledbl() != tempEnabledBL){
//        CORE_INFO("Changed");
//      }
    }

    void LaunchDaemonsManager::fetchAllDaemons() {

      std::vector<HummingBirdCore::Utils::File> files = HummingBirdCore::Utils::FolderUtils::getFilesInFolder(c_userAgentPath.string(), ".plist");

      for (const auto &file: files) {
        LaunchDaemon *da = new LaunchDaemon(file, LaunchDaemonStatus::Unknown);
        m_userAgent.push_back(da);
      }
    }

    void LaunchDaemonsManager::renderDaemonsTable(std::vector<LaunchDaemon *> daemons) {
      ImGui::BeginTable("LaunchDaemonsTable", 3, ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders);

      ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed);
      ImGui::TableSetupColumn("Enable", ImGuiTableColumnFlags_WidthFixed);
      ImGui::TableSetupColumn("Status", ImGuiTableColumnFlags_WidthFixed);

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

        {
          ImGui::TableNextColumn();
          ImGui::Text("STATUS");
        }

        ImGui::PopID();
      }

      ImGui::EndTable();
    }

    void LaunchDaemonsManager::selectDaemon(LaunchDaemon *daemon) {
      //TODO: Check If current is unsaved else show modal
      m_selectedDaemon = daemon;
      m_selectedDaemon->init();
    }
  }// namespace System
}// namespace HummingBirdCore