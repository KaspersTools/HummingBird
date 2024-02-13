//
// Created by Kasper de Bruin on 31/01/2024.
//

#include "SqlWindow.h"
#include <Hummingbird_SQL/Structures.h>
//#include <KDB_ImGui/Extension.h>
#include <imgui.h>

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
      const HummingBird::Sql::SchemaInfo currentSchema = m_connection.getCurrentSchema();
      const HummingBird::Sql::TableInfo currentTable = currentSchema.getCurrentTable();
      const std::vector<HummingBird::Sql::Row> currentTableRows = currentTable.getRows();
      const auto schemas = m_connection.getSchemaNames();

      const int columnCount = currentTable.getColumnCount() - 1;
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


      if (schemas.empty()) {
        ImGui::BeginChild("##left", ImVec2(0, 0), true);
//        KDB_IMGUI::textCenteredMultiline("No Schemas found", ImVec4(1,0,0,1));
        ImGui::EndChild();
      } else {
        if (m_initialSize.x == -1) {
          for (const auto &schema: schemas) {
            const ImVec2 textSize = ImGui::CalcTextSize(schema.c_str());
            if (textSize.x > m_initialSize.x) {
              m_initialSize.x = textSize.x;
            }
          }
        }
        //Begin left layout for schemas
        const ImGuiChildFlags leftChildFlags = ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX;
        const ImGuiWindowFlags leftWindowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |
                                                 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;

        ImGui::BeginChild("##left", m_initialSize, leftChildFlags, leftWindowFlags);
        {
          //HEADER That sticks on top
          {
            std::string headerText = "Schemas for Server: " + m_connection.getHostname();
            ImGui::Text(headerText.c_str());
          }

          ImGuiChildFlags leftSchemaChildFlags = ImGuiChildFlags_Border;

          ImGui::BeginChild("##leftSchemas", {});
          //Schemasx
          {
            const ImGuiTreeNodeFlags c_treeNodeFlagsLeft = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
            const ImGuiSelectableFlags c_selectableFlags = ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_SpanAllColumns;
            for (const auto &schema: schemas) {
              ImGui::PushID(schema.c_str() + 1);
              {
                bool dbNodeOpen = ImGui::TreeNodeEx(schema.c_str(), c_treeNodeFlagsLeft);
                if (dbNodeOpen) {
                  const std::vector<std::string> &tables = m_connection.getSchema(schema).getTableNames();
                  for (const auto &table: tables) {
                    const bool selected = m_connection.getCurrentSchema().isTableSet() && m_connection.getCurrentSchema().getCurrentTable().getName() == table;

                    if (ImGui::Selectable(table.c_str(),
                                          selected, c_selectableFlags)) {
                      m_connection.setTable(schema, table);
                    }
                  }
                  ImGui::TreePop();
                }
              }
              ImGui::PopID();
            }
          }
          ImGui::EndChild();
        }
        ImGui::EndChild();
        ImGui::SameLine();

        ImGui::BeginChild("##rightChild", ImVec2(0,0), true);
        {
          const std::string headerText = "Table: " + currentTable.getName() + " in Schema: " + currentSchema.getName();
          ImGui::Text(headerText.c_str());
          ImGui::BeginChild("##rightChildTable", {});
          {
            ImGui::Separator();
            {
              if (!m_connection.isCurrentSchemaSet() || !m_connection.getCurrentSchema().isTableSet()) {
//                KDB_IMGUI::textCenteredMultiline("No Schema or Table selected", ImVec4(1,0,0,1));
              } else if (columnCount != -1) {
//                KDB_IMGUI::textCenteredMultiline("No Columns found, for table: " + currentTable.getName(), ImVec4(1,0,0,1));
              } else {
                ImGuiTableFlags tableFlags = ImGuiTableFlags_Resizable |
                                             ImGuiTableFlags_RowBg |
                                             ImGuiTableFlags_BordersOuter |
                                             ImGuiTableFlags_BordersV |
                                             ImGuiTableFlags_Reorderable;
                ImGuiTableColumnFlags columnFlags = ImGuiTableColumnFlags_NoClip;

                if (ImGui::BeginTable("##table", columnCount, tableFlags)) {
                  ImGui::TableSetupScrollFreeze(columnCount, 1);
                  for (int i = 0; i < columnCount; i++) {
                    const std::string columnName = currentTable.getRows()[0].getColumn(i).getName();
                    ImGui::TableSetupColumn(columnName.c_str());
                  }
                  ImGui::TableHeadersRow();

                  for (const auto &row: currentTableRows) {
                    ImGui::TableNextRow();
                    for (int i = 0; i < columnCount; i++) {
                      const std::string value = row.getColumn(i).getValueAsString();
                      ImGui::TableSetColumnIndex(i);
                      ImGui::Text(value.c_str());
                    }
                  }

                  ImGui::EndTable();
                }
              }
            }
          }
          ImGui::EndChild();
        }
        ImGui::EndChild();
      }
    }
  }
}// namespace HummingBirdCore
