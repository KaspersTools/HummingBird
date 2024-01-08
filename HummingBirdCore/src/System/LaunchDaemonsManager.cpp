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

      ImGui::BeginChild("LaunchDaemonsManagerRightPanel", ImVec2(0, 0), true);

      if (m_selectedDaemon != nullptr) {
        ImGui::BeginChild("SelectedDeamonLeftXML", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 0), c_leftChildFlags, c_leftWindowFlags);
        ImGui::Text("Selected Daemon");
        ImGui::Checkbox("Wrap Text", &m_wrapText);
        ImGui::Separator();

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyle().Colors[ImGuiCol_WindowBg]);
        ImGui::BeginChild("SelectedDaemonLeftXMLChild", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
        if (m_wrapText) {
          ImGui::TextWrapped(m_selectedDaemon->getFile().content.c_str());
        }else{
          ImGui::Text(m_selectedDaemon->getFile().content.c_str());
        }
        ImGui::EndChild();
        ImGui::PopStyleColor();
        ImGui::PopStyleVar(5);

        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginChild("SelectedDeamonRight", ImVec2(0, 0), true);
        ImGui::Text("Selected Daemon");
        ImGui::Separator();
        ImGui::EndChild();
      }

      ImGui::EndChild();
      ImGui::EndGroup();
    }

    void LaunchDaemonsManager::fetchAllDaemons() {

      std::vector<HummingBirdCore::Utils::File> files = HummingBirdCore::Utils::FolderUtils::getFilesInFolder(c_userAgentPath.string(), ".plist");

      for (const auto &file: files) {
        LaunchDaemon *da = new LaunchDaemon(file, false, LaunchDaemonStatus::Unknown);
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
      m_selectedDaemon->readPlist();
      m_selectedDaemon->init();
    }
  }// namespace System
}// namespace HummingBirdCore