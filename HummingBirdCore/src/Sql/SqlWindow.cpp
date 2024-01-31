//
// Created by Kasper de Bruin on 31/01/2024.
//

#include "SqlWindow.h"

namespace HummingBirdCore {
  void SqlWindow::render() {
    if (!m_connection.isConnected()) {
      ImGui::InputText("Server Name", &m_inputServerName);
      ImGui::InputText("User Name", &m_inputUserName);
      ImGui::InputText("Password", &m_inputPassword);
      if (ImGui::Button("Connect")) {
        m_connection.connect(m_inputServerName, m_inputUserName, m_inputPassword, "", 33060);
      }
    } else {
      //Begin horizontal layout
      ImGuiWindowFlags childWindowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
      {
        ImGui::BeginChild("##top", ImVec2(0, ImGui::GetStyle().FramePadding.y * 2.0f + ImGui::GetFrameHeightWithSpacing()), true, childWindowFlags);
        if (ImGui::Button("Disconnect")) {
          m_connection.disconnect();
        }
        ImGui::SameLine();
        ImGui::EndChild();
      }

      //Begin left layout for database list
      {
        ImGui::BeginChild("##left", ImVec2(ImGui::GetWindowWidth() * 0.2f, 0), true, childWindowFlags);
        ImGui::Text("Databases");
        ImGui::Separator();
        {
          ImGui::BeginChild("##leftScrollingRegion", ImVec2(0, 0), true, childWindowFlags);

          ImGui::EndChild();
        }
        ImGui::EndChild();
      }
      ImGui::SameLine();
      //Begin right layout for table data
      {
        ImGui::BeginChild("##right", ImVec2(0, 0), true, childWindowFlags);
        ImGui::Text("Data");
        ImGui::Separator();
        {
          ImGui::BeginChild("##rightScrollingRegion", ImVec2(0, 0), true, childWindowFlags);

          ImGui::EndChild();
        }
        ImGui::EndChild();
      }
    }
  }
}// namespace HummingBirdCore
