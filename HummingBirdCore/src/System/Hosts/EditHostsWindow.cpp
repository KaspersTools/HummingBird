//
// Created by Kasper de Bruin on 01/11/2023.
//


#include "EditHostsWindow.h"

void HummingBirdCore::System::EditHostsWindow::render() {

#ifdef __APPLE__
  if(ImGui::Button("Save")){

    std::string finalString;

    for (int row = 0; row < m_hostsFileLines.size(); row++) {
      if (m_hostsFileLines[row].isCommnent())
      {
        continue;
      }
      if(!m_hostsFileLines[row].enabled)
      {
        finalString += "#";
      }

      finalString += m_hostsFileLines[row].getIP() + " " + m_hostsFileLines[row].getHostname() + "\n";
    }

    // Set the SUDO_ASKPASS environment variable
    setenv("SUDO_ASKPASS", c_askPassPath.c_str(), 1);

    //che
    // Write the host entries to a temporary file
    std::ofstream tempFile(c_tempFilePath);
    if (!tempFile.is_open()) {
      std::cerr << "Failed to open temporary file for writing." << std::endl;
      return;
    }

    tempFile << finalString;

    tempFile.close();

    // Use sudo with the -A option to trigger askpass
    std::string command = "sudo -A mv " + c_tempFilePath + " " + c_hostsPath;
    int result = system(command.c_str());
//    int result = system(command.c_str());

    if (result != 0) {
      std::cerr << "Failed to write to " << c_hostsPath << ". Ensure you have sudo privileges." << std::endl;
    } else {
      std::cout << "Successfully wrote to " << c_hostsPath << "." << std::endl;
    }
  }
#else
  ImGui::ColorButton("Saving not supported on other platforms then mac os", ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
#endif

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
