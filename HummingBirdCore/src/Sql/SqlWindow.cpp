//
// Created by Kasper de Bruin on 29/12/2023.
//

#include "SqlWindow.h"
#include <imgui.h>
#include <imgui_stdlib.h>

namespace HummingBirdCore {
  namespace Sql {

    void SqlWindow::render() {
      ImGui::InputText("Host", &m_inputhost);
      ImGui::InputText("User", &m_inputuser);
      ImGui::InputText("Password", &m_inputpassword);
      ImGui::InputInt("Port", (int *) &m_port);

      if (m_connection.getIsConnected()) {
        ImGui::Text("Connected to database");
        ImGui::SameLine();
        if (ImGui::Button("Disconnect")) {
          m_connection.disconnect();
        }
      } else {
        ImGui::Text("Not connected to database");
        ImGui::SameLine();
        if (ImGui::Button("Connect")) {
          m_connection.connect(m_inputhost, m_inputuser, m_inputpassword, "", m_port);
        }
      }

      if (ImGui::BeginTabBar("Tabs")) {
        if (ImGui::BeginTabItem("Data")) {
          renderDataTab();
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Logging")) {
          ImGui::BeginChild("Logs", ImVec2(0, 0), true);
          for (auto &log: m_logSink->getLogMessages()) {
            ImColor color = HummingBirdCore::Logging::getLogColor(log.level);
            ImGui::TextColored(color, log.message.c_str());
          }
          ImGui::EndChild();
          ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
      }
    }

    void SqlWindow::renderQueryTab() {
      ImGui::BeginChild("Query", ImVec2(0, 0), true);
      ImGui::EndChild();
    }

    void SqlWindow::renderTablesTab() {
      ImGui::BeginChild("Tables", ImVec2(0, 0), true);
      ImGui::EndChild();
    }

    void SqlWindow::renderDatabasesTab() {
      ImGui::BeginChild("Databases", ImVec2(0, 0), true);
      ImGui::EndChild();
    }

    void SqlWindow::renderDataTab() {
      if (!m_connection.getIsConnected()) {
        ImGui::TextColored(ImColor(255, 0, 0), "Not connected to database");
        return;
      }

      const std::vector<Database> &dbs = m_connection.getDatabases();
      const int currentDbIndex = m_connection.getCurrentDatabaseIndex();
      const int currentTableIndex = m_connection.getCurrentDatabase().getCurrentTableIndex();

      //Left window
      getRenderStats(c_renderStatsLeftName)->startUpdateStats();
      {
        ImGui::BeginChild("ChildLeft", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 0), c_leftChildFlags, c_leftWindowFlags);
        for (int i = 0; i < dbs.size(); ++i) {
          bool dbNodeOpen = ImGui::TreeNodeEx(dbs[i].getName().c_str(), c_baseTreeNodeFlagsLeft);

          if (dbNodeOpen) {
            for (int j = 0; j < dbs[i].getTables().size(); ++j) {
              const std::string tableName = dbs[i].getTables()[j]->name;
              bool isSelectedTable = currentDbIndex == i && currentTableIndex == j;

              if (ImGui::Selectable(tableName.c_str(), isSelectedTable, c_selectableFlags)) {
                if(m_connection.useDatabase(i, j)) {
                  m_dataViewer->setTable(dbs[i].getTables()[j]);
                }
              }
            }
            ImGui::TreePop();
          }
        }
        ImGui::EndChild();
      }
      getRenderStats(c_renderStatsLeftName)->endUpdateStats();

      //Right window
      getRenderStats(c_renderStatsRightName)->startUpdateStats();
      {
        ImGui::SameLine();
        m_dataViewer->render();
      }
      getRenderStats(c_renderStatsRightName)->endUpdateStats();
    }
  }// namespace Sql
}// namespace HummingBirdCore
