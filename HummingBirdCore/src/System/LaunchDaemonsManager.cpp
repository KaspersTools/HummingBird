//
// Created by Kasper de Bruin on 08/01/2024.
//

#include "LaunchDaemonsManager.h"

namespace HummingBirdCore {
  namespace System {
    void LaunchDaemonsManager::render() {
      ImGui::BeginChild("LaunchDaemons", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 0), c_leftChildFlags, c_leftWindowFlags);
      ImGui::Text("LaunchDaemons");
      ImGui::Separator();

      //tab
      if (ImGui::BeginTabBar("LaunchDaemonsTabBar")) {
        if (ImGui::BeginTabItem("User Agent")) {
          renderDaemonsTable(m_userAgent);
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Global Agent")) {
          renderDaemonsTable(m_globalAgent);
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Global Daemons")) {
          renderDaemonsTable(m_globalDaemons);
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("System Agent")) {
          renderDaemonsTable(m_systemAgent);
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("System Daemons")) {
          renderDaemonsTable(m_systemDaemons);
          ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
      }

      ImGui::EndChild();

      //Right resizeable panel
      ImGui::SameLine();
      ImGui::BeginGroup();

      ImGui::BeginChild("LaunchDaemonsManagerRightPanel", ImVec2(0, -ImGui::GetFrameHeightWithSpacing() * 2), true);
      std::string title = "LaunchDaemon: ";

      if (m_selectedDaemon != NULL) {
        title += m_selectedDaemon->getName();
        ImGui::TextColored(ImVec4(1, 1, 1, 1), title.c_str());
      } else {
        title += "None";
        ImGui::TextColored(ImVec4(1, 1, 0, 1), title.c_str());
      }

      ImGui::Separator();

      if (m_selectedDaemon == NULL) {
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "No LaunchDaemon selected");
      } else {
        if (ImGui::Button("Save")) {
          CORE_WARN("Saving daemon not implemented yet");
          CORE_INFO("Saving daemon: " + m_selectedDaemon->getName());
          //TODO: Save to file and show modal
        }
        if (ImGui::Button("Revert")) {
          CORE_WARN("Reverting daemon not implemented yet");
          CORE_INFO("Reverting daemon: " + m_selectedDaemon->getName() + " to original state");
          //TODO: Show modal
        }
        ImGui::Text("Name: %s", m_selectedDaemon->getName().c_str());
        ImGui::Text("Path: %s", m_selectedDaemon->getFile().path.c_str());
        if (ImGui::Checkbox("Enabled", &m_selectedDaemon->getEnabled())) {
            m_selectedDaemon->setEnabled(m_selectedDaemon->getEnabled());
        }

        ImGui::Text("Status: %s", m_selectedDaemon->getStatus() ==
                                                  LaunchDaemonStatus::Running
                                          ? "Running"
                                  : m_selectedDaemon->getStatus() ==
                                                  LaunchDaemonStatus::Stopped
                                          ? "Stopped"
                                          : "Unknown");
      }

      ImGui::EndChild();
      ImGui::EndGroup();
    }

    void LaunchDaemonsManager::fetchAllDaemons() {

      std::vector<HummingBirdCore::Utils::File> files = HummingBirdCore::Utils::FolderUtils::getFilesInFolder(c_userAgentPath.string(), ".plist");

      for (const auto &file: files) {
        LaunchDaemon *da = new LaunchDaemon(file.name, file, false, LaunchDaemonStatus::Unknown);
        m_userAgent.push_back(da);
      }
    }

    void LaunchDaemonsManager::renderDaemonsTable(std::vector<LaunchDaemon *> daemons) {
      ImGui::BeginTable("LaunchDaemonsTable", 3, ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders);

      ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed);
      ImGui::TableSetupColumn("Enabled", ImGuiTableColumnFlags_WidthFixed);
      ImGui::TableSetupColumn("Status", ImGuiTableColumnFlags_WidthFixed);

      ImGui::TableSetupScrollFreeze(0, 1);// Make row(header) always visible
      ImGui::TableHeadersRow();

      for (LaunchDaemon *daemon: daemons) {
        ImGui::PushID(daemon->getName().c_str());

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        if (ImGui::Selectable(daemon->getName().c_str(), *m_selectedDaemon == *daemon)) {
          selectDaemon(daemon);
        }
        ImGui::TableNextColumn();
        if (ImGui::Checkbox("Enabled", &daemon->getEnabled())) {
          daemon->setEnabled(daemon->getEnabled());
        }
        ImGui::TableNextColumn();
        ImGui::Text(daemon->getStatus() == LaunchDaemonStatus::Running ? "Running" : daemon->getStatus() == LaunchDaemonStatus::Stopped ? "Stopped"
                                                                                                                                        : "Unknown");
        ImGui::PopID();
      }

      ImGui::EndTable();
    }

    void LaunchDaemonsManager::selectDaemon(LaunchDaemon *daemon) {
      //TODO: Check If current is unsaved else show modal
      m_selectedDaemon = daemon;
    }
  }// namespace System
}// namespace HummingBirdCore