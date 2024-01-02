//
// Created by Kasper de Bruin on 29/12/2023.
//

#include "SqlWindow.h"
#include <imgui.h>
#include <imgui_stdlib.h>

namespace HummingBirdCore {
  namespace Sql {

    void SqlWindow::render() {
      ImGui::InputText("Host", &m_host);
      ImGui::InputText("User", &m_user);
      ImGui::InputText("Password", &m_password);
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
          m_connection.connect(m_host, m_user, m_password, "", m_port);
        }
      }

      if (ImGui::BeginTabBar("Tabs")) {
        if (ImGui::BeginTabItem("Query")) {
          renderQueryTab();
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Databases")) {
          renderDatabasesTab();
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Tables")) {
          renderTablesTab();
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Columns")) {
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Data")) {
          renderDataTab();
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Errors")) {
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Warnings")) {

          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Info")) {

          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Debug")) {
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
      ImGui::InputTextMultiline("##Query", &m_queryInput);
      ImGui::SameLine();
      ImGui::Dummy(ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 16));
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

      std::vector<Database> dbs = m_connection.getDatabases();

      Database currentDb = m_connection.getCurrentDatabase();
      std::vector<Table> tables = currentDb.getTables();
      Table curTable = tables[currentDb.getCurrentTableIndex()];

      //Left window
      {
        ImGui::BeginChild("ChildLeft", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 0), c_leftChildFlags, c_leftWindowFlags);
        for (int i = 0; i < dbs.size(); ++i) {
          bool dbNodeOpen = ImGui::TreeNodeEx(dbs[i].getName().c_str(), c_baseTreeNodeFlagsLeft);

          if (dbNodeOpen) {
            for (int j = 0; j < dbs[i].getTables().size(); ++j) {
              const std::string tableName = dbs[i].getTables()[j].name;
              bool isSelectedTable = currentDb.getCurrentTableIndex() == j;

              if (ImGui::Selectable(tableName.c_str(), isSelectedTable, c_selectableFlags)) {
                m_connection.useDatabase(i, j);
                CORE_TRACE("Selected table {0}", tableName);
              }

              if (j >= m_settings.c_defaultMaxTableSize) {
                break;
              }
            }
            ImGui::TreePop();
          }

          if (i >= m_settings.c_defaultMaxDatabaseSize) {
            break;
          }
        }
        ImGui::EndChild();
      }

      //Right window
      {
        ImGui::SameLine();
        ImGui::BeginChild("ChildRight", ImVec2(0, 0), c_rightChildFlags, c_rightWindowFlags);

        const int headerCount = currentDb.getTables()[currentDb.getCurrentTableIndex()].headers.size();
        const int rowCount = currentDb.getTables()[currentDb.getCurrentTableIndex()].rows.size();

        if (headerCount != 0) {
          if (ImGui::BeginTable("Table Data Table", headerCount, ImGuiTableFlags_Resizable)) {
            ImGui::TableSetupScrollFreeze(0, 1);// Make top row always visible

            for (int i = 0; i < headerCount; ++i) {
              ImGui::TableSetupColumn(curTable.headers[i].name.c_str(), ImGuiTableColumnFlags_WidthFixed, 150.0f);
            }
            ImGui::TableHeadersRow();

            for (int i = 0; i < rowCount; ++i) {
              ImGui::TableNextRow();
              for (int j = 0; j < curTable.rows[i].data.size(); ++j) {
                ImGui::TableSetColumnIndex(j);
                ImGui::Text(curTable.rows[i].data[j].c_str());
              }

              if (i >= m_settings.c_defaultMaxDataSize) {
                break;
              }
            }

            ImGui::EndTable();
          } else {
            ImGui::TextColored(ImColor(255, 0, 0), "No headers found");
          }
        }

        ImGui::EndChild();
      }


      //      ImGui::EndChild();
    }
  }// namespace Sql
}// namespace HummingBirdCore
