//
// Created by Kasper de Bruin on 01/11/2023.
//

#include "EditHostsWindow.h"
#include <Security/Security.h>

void HummingBirdCore::System::EditHostsWindow::Render() {
  //Make imgui table
  if(ImGui::Button("Save")){
    std::string finalString;

    for (int row = 0; row < m_hostsFileLines.size(); row++) {
      if (m_hostsFileLines[row].isCommnent())
      {
//        finalString += m_hostsFileLines[row].getHostname() + "\n";
        continue;
      }
      if(!m_hostsFileLines[row].enabled)
      {
        finalString += "#";
      }

      finalString += m_hostsFileLines[row].getIP() + " " + m_hostsFileLines[row].getHostname() + "\n";
    }

    CORE_TRACE("Created final string for host file: {0}", finalString);

    if(FileUtils::writeToFileWithAuthorization("/etc/hosts", finalString)){
      CORE_INFO("Successfully wrote to file {} with authorization", "/etc/hosts");
    }else
    {
      CORE_ERROR("Failed to write to file {} with authorization", "/etc/hosts");
    }


  }
  if (ImGui::BeginTable("Hosts", 6, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable)) {
    ImGui::TableSetupColumn("Enabled");
    ImGui::TableSetupColumn("IP");
    ImGui::TableSetupColumn("Hostname");
    ImGui::TableSetupColumn("Actions");
    ImGui::TableHeadersRow();

    for (int row = 0; row < m_hostsFileLines.size(); row++) {
      if (m_hostsFileLines[row].isCommnent())
        continue;

      ImGui::TableNextRow();// Move to the next row


      int column = 0;

      ImGui::TableSetColumnIndex(column++);
      ImGui::Checkbox("", &m_hostsFileLines[row].enabled);

      ImGui::TableSetColumnIndex(column++);
      char ipBuffer[256];
      strncpy(ipBuffer, m_hostsFileLines[row].getIP().c_str(), sizeof(ipBuffer));
      ipBuffer[sizeof(ipBuffer) - 1] = 0;// Ensure null termination
      std::string uidIP = m_hostsFileLines[row].getOriginalIP() + std::to_string(row) + std::to_string(column);
      std::string labelip = "##" + uidIP;
      if (ImGui::InputText(labelip.c_str(), ipBuffer, sizeof(ipBuffer))) {
        m_hostsFileLines[row].setIP(ipBuffer);
      }

      ImGui::TableSetColumnIndex(column++);
      char hostnameBuffer[256];
      strncpy(hostnameBuffer, m_hostsFileLines[row].getHostname().c_str(), sizeof(hostnameBuffer));
      hostnameBuffer[sizeof(hostnameBuffer) - 1] = 0;// Ensure null termination
      std::string uidHostname = m_hostsFileLines[row].getOriginalHostname() + std::to_string(row) + std::to_string(column);
      std::string labelhostname = "##" + uidHostname;
      if (ImGui::InputText(labelhostname.c_str(), hostnameBuffer, sizeof(hostnameBuffer))) {
        m_hostsFileLines[row].setHostname(hostnameBuffer);
      }

      ImGui::TableSetColumnIndex(column++);
      if (ImGui::Button("Reset IP")) {
        m_hostsFileLines[row].setIP(m_hostsFileLines[row].getOriginalIP().c_str());
      }

      ImGui::TableSetColumnIndex(column++);
      if (ImGui::Button("Reset Hostname")) {
        m_hostsFileLines[row].setHostname(m_hostsFileLines[row].getOriginalHostname().c_str());
      }

      ImGui::TableSetColumnIndex(column++);

      if (ImGui::Button("Reset")) {
        m_hostsFileLines[row].reset();
      }
    }

    ImGui::EndTable();
  }
}
