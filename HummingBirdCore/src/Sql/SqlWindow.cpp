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
      ImGui::InputText("Database", &m_database);
      ImGui::InputInt("Port", (int *) &m_port);

      if (m_connection.isConnected()) {
        ImGui::Text("Connected to database %s", m_database.c_str());
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

    void SqlWindow::renderQueryTab() {
      ImGui::BeginChild("Query", ImVec2(0, 0), true);
      ImGui::InputTextMultiline("##Query", &m_queryInput);
      ImGui::SameLine();
      //result if its a success or not
      if (m_queryResult.success) {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Success");
        ImGui::SameLine();
        ImGui::Text(m_queryResult.source.c_str());
        ImGui::SameLine();
        std::string rows = "rows: " + std::to_string(m_queryResult.rowCount);
        ImGui::Text(rows.c_str());
        ImGui::SameLine();
        std::string fields = "fields: " + std::to_string(m_queryResult.fieldCount);
        ImGui::Text(fields.c_str());
      } else {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Failed");
        ImGui::SameLine();
        ImGui::Text(m_queryResult.source.c_str());
        ImGui::SameLine();
        ImGui::Text(m_queryResult.error.c_str());
      }
      if (ImGui::Button("Execute")) {
        m_queryResult = m_connection.query(m_queryInput);
      }

      //Table with the query result
      //TODO: MAKE OWN IMGUI WIDGET SO WE CAN REUSE THIS
      if (m_queryResult.success) {
        ImGui::BeginChild("QueryResult", ImVec2(0, 0), true);
        const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

        static ImGuiTableFlags flags = ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
        static int freeze_cols = 1;
        static int freeze_rows = 1;
        ImGuiStyle &style = ImGui::GetStyle();

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x, (float) (int) (style.FramePadding.y * 0.60f)));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(style.ItemSpacing.x, (float) (int) (style.ItemSpacing.y * 0.60f)));
        ImGui::CheckboxFlags("ImGuiTableFlags_ScrollX", &flags, ImGuiTableFlags_ScrollX);
        ImGui::SetNextItemWidth(ImGui::GetFrameHeight());
        ImGui::DragInt("freeze_cols", &freeze_cols, 0.2f, 0, 9, NULL, ImGuiSliderFlags_NoInput);
        ImGui::SetNextItemWidth(ImGui::GetFrameHeight());
        ImGui::DragInt("freeze_rows", &freeze_rows, 0.2f, 0, 9, NULL, ImGuiSliderFlags_NoInput);
        ImGui::PopStyleVar(2);

        ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 19);
        if (ImGui::BeginTable("query_table", m_queryResult.columnNames.size(), flags, outer_size)) {
          ImGui::TableSetupScrollFreeze(freeze_cols, freeze_rows);
          for (auto &column: m_queryResult.columnNames) {
            ImGui::TableSetupColumn(column.c_str());
          }
          ImGui::TableHeadersRow();

          for (auto &row: m_queryResult.data) {
            for (auto &column: row) {
              ImGui::TableNextColumn();
              ImGui::Text(column.c_str());
            }
          }
          ImGui::EndTable();
        }
        ImGui::EndChild();
      }
      ImGui::EndChild();
    }

    void SqlWindow::renderTablesTab() {
      const QueryResult tables = m_connection.getLastTableQuery();
      ImGui::BeginChild("Tables", ImVec2(0, 0), true);

      const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
      ImGuiTableFlags flags = ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
      ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 19);

      if (ImGui::BeginTable("query_table", tables.columnNames.size(), flags, outer_size)) {
        ImGui::TableSetupScrollFreeze(true, true);
        for (auto &column: tables.columnNames) {
          ImGui::TableSetupColumn(column.c_str());
        }
        ImGui::TableHeadersRow();

        for (auto &row: tables.data) {
          for (auto &column: row) {
            ImGui::TableNextColumn();
            ImGui::Text(column.c_str());
          }
        }
        ImGui::EndTable();
      }
      ImGui::EndChild();
    }

    //TODO: ADD BUTTONS TO SWITCH TO ETC.ETC.
    void SqlWindow::renderDatabasesTab(){
        const QueryResult databases = m_connection.getLastDatabaseNames();
        ImGui::BeginChild("Databases", ImVec2(0, 0), true);

        const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
        ImGuiTableFlags flags = ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
        ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 19);

        if (ImGui::BeginTable("databasestable", databases.columnNames.size(), flags, outer_size)) {
            ImGui::TableSetupScrollFreeze(true, true);
            for (auto &column: databases.columnNames) {
            ImGui::TableSetupColumn(column.c_str());
            }
            ImGui::TableHeadersRow();

            for (auto &row: databases.data) {
            for (auto &column: row) {
                ImGui::TableNextColumn();
                ImGui::Text(column.c_str());
            }
            }
            ImGui::EndTable();
        }
        ImGui::EndChild();
    }
  }// namespace Sql
}// namespace HummingBirdCore