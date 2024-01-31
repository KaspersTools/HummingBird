//
// Created by Kasper de Bruin on 31/01/2024.
//

#include "SqlWindow.h"
#include <Hummingbird_SQL/Structures.h>

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
      const ImGuiWindowFlags c_topWindowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
      //Begin horizontal layout
      {
        ImGui::BeginChild("##top", ImVec2(0, ImGui::GetStyle().FramePadding.y * 2.0f + ImGui::GetFrameHeightWithSpacing()), true, c_topWindowFlags);
        if (ImGui::Button("Disconnect")) {
          m_connection.disconnect();
        }
        ImGui::SameLine();
        ImGui::EndChild();
      }

      //Begin left layout for database list

      const ImGuiChildFlags leftChildFlags = ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX;
      const ImGuiWindowFlags leftWindowFlags = ImGuiWindowFlags_HorizontalScrollbar;
      if (m_lastMaxWidth == -1) {
        m_lastMaxWidth = ImGui::GetContentRegionAvailWidth() * .2f;
      }

      ImGui::BeginChild("##left", ImVec2(0, 0), leftChildFlags, leftWindowFlags);
      {
        ImGui::Text("Databases");
        ImGui::Separator();
        {

          ImGui::BeginChild("##leftScrollingRegion", ImVec2(0, 0), true);
          {
            const ImGuiTreeNodeFlags c_treeNodeFlagsLeft = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
            const ImGuiSelectableFlags c_selectableFlags = ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_SpanAllColumns;

            for (HummingBird::Sql::SchemaInfo *schema: m_connection.getSchemas()) {
              bool dbNodeOpen = ImGui::TreeNodeEx(schema->name.c_str(), c_treeNodeFlagsLeft);
              if (dbNodeOpen) {
                for (HummingBird::Sql::TableInfo *table: m_connection.getTables(schema)) {
                  bool isSelectedTable = table == m_connection.getCurrentTable();
                  if (ImGui::Selectable(table->name.c_str(), isSelectedTable, c_selectableFlags)) {
                    m_connection.setSchema(schema);
                    m_connection.setTable(table);
                  }
                }
                ImGui::TreePop();
              }
            }
          }
          ImGui::EndChild();
        }
      }
      ImGui::EndChild();

      ImGui::SameLine();

      ImGui::BeginChild("##right", ImVec2(0, 0), true);
      {
        ImGui::Text("Data");
        ImGui::Separator();
        {
          ImGui::BeginChild("##rightScrollingRegion", ImVec2(0, 0), true);
          {
            if (m_connection.getCurrentSchema() != nullptr) {
              if (m_connection.getCurrentTable() != nullptr) {
                std::vector<std::string> columnNames = m_connection.getCurrentColumnLayout();
                ImGui::Columns(m_connection.getCurrentTable()->columns.size());

                for (const auto &column: columnNames) {
                  ImGui::Text("%s", column.c_str());
                  ImGui::NextColumn();
                }
                ImGui::Separator();

                for (const auto &row: m_connection.getCurrentTable()->rows) {
                  for (const auto &column: columnNames) {
                    ImGui::Text("%s", row.getColumnValueAsString(column).c_str());
                    ImGui::NextColumn();
                  }
                }
              }
            }
          }
          ImGui::EndChild();
        }
      }
        ImGui::EndChild();
    }
  }
}// namespace HummingBirdCore
