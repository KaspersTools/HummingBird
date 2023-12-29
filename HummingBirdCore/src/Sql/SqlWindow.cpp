//
// Created by Kasper de Bruin on 29/12/2023.
//

#include "SqlWindow.h"

namespace HummingBirdCore {
  namespace Sql {

    void SqlWindow::render() {
      ImGui::InputText("Host", &m_host);
      ImGui::InputText("User", &m_user);
      ImGui::InputText("Password", &m_password);
      ImGui::InputText("Database", &m_database);
      ImGui::InputInt("Port", (int *) &m_port);

      if (m_connection.isConnected()) {
        ImGui::Text("Connected to database");
        ImGui::SameLine();
        if (ImGui::Button("Disconnect")) {
          m_connection.disconnect();
        }
      } else {
        ImGui::Text("Not connected to database");
        ImGui::SameLine();
        if (ImGui::Button("Connect")) {
          m_connection.connect(m_host, m_user, m_password, m_database, m_port);
        }
      }

      if (ImGui::BeginTabBar("Tabs")) {
        if(ImGui::BeginTabItem("Query"))
        {
          ImGui::EndTabItem();
        }
        if(ImGui::BeginTabItem("Tables"))
        {
          ImGui::EndTabItem();
        }
        if(ImGui::BeginTabItem("Columns"))
        {
          ImGui::EndTabItem();
        }
        if(ImGui::BeginTabItem("Data"))
        {
          ImGui::EndTabItem();
        }
        if(ImGui::BeginTabItem("Errors"))
        {
          ImGui::EndTabItem();
        }
        if(ImGui::BeginTabItem("Warnings"))
        {

          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Info")) {

          ImGui::EndTabItem();
        }
        if(ImGui::BeginTabItem("Debug"))
        {
          ImGui::EndTabItem();
        }
        if(ImGui::BeginTabItem("Logging"))
        {
          ImGui::BeginChild("Logs", ImVec2(0, 0), true);
          for (auto &log: m_logSink->getLogMessages()) {
            switch (log.level) {
              case spdlog::level::info:
                ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), log.message.c_str());
                break;
              case spdlog::level::warn:
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), log.message.c_str());
                break;
              case spdlog::level::err:
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), log.message.c_str());
                break;
              case spdlog::level::debug:
                ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), log.message.c_str());
                break;
              case spdlog::level::critical:
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), log.message.c_str());
                break;
              case spdlog::level::trace:
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), log.message.c_str());
                break;
              default:
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), log.message.c_str());
                break;
            }
          }
          ImGui::EndChild();
          ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
      }
    }
  }// namespace Sql
}// namespace HummingBirdcore