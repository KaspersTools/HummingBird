//
// Created by Kasper de Bruin on 02/01/2024.
//

#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <mysql.h>

#include "Log.h"
#include "UIWindows/UIWindow.h"

namespace HummingBirdCore::Widgets {
  struct Header {
    std::string name;
    enum_field_types type;
  };

  struct Row {
    int id;
    std::vector<std::string> data;
  };

  struct Table {
    std::string name;
    std::string databaseName;
    bool isInitialized = false;

    std::vector<Header> headers;
    std::vector<Row> rows;

    uint currentColumnIndex = 0;
    std::string currentColumnName = "";

    uint currentRowIndex = 0;
    std::string currentRowName = "";

    int numOfRows() const {
      return rows.size();
    }

    int numOfHeaders() const {
      return headers.size();
    }

    void setHeadersAndRows(const std::vector<Header> &headers, const std::vector<Row> &rows) {
      this->headers = headers;
      this->rows = rows;
      isInitialized = true;
    }
  };

  class DataViewer : public UIWindow {
public:
    explicit DataViewer(const std::string &name) : UIWindow(ImGuiWindowFlags_MenuBar, name) {

    }
    ~DataViewer() = default;
    void render() override {
      ImGui::BeginChild("DataDisplay", ImVec2(0, -ImGui::GetFrameHeightWithSpacing() * 2), true);
      {
        if (m_table == nullptr) {
          ImGui::Text("No table selected");
        } else {
          if (!m_table->isInitialized) {
            ImGui::Text("No table selected");
          } else if (m_table->numOfHeaders() == 0) {
            ImGui::Text("No headers found");
          } else {
            ImGuiTableFlags flags =
                    ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable
                    | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable
                    | ImGuiTableFlags_RowBg
                    | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV
                    | ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_ScrollY;

            ImGuiTableColumnFlags columns_base_flags = ImGuiTableColumnFlags_None;
            float textBaseHeight = ImGui::GetTextLineHeightWithSpacing();
            float rowMinHeight = 0.0f;    // Auto

            int numOfDisplayedItems = 0;

            if (ImGui::BeginTable("Table Data Table", m_table->numOfHeaders(), flags, ImVec2(0.0f, textBaseHeight * 15))) {
              {
                for (int i = 0; i < m_table->numOfHeaders(); ++i) {
                  ImGui::TableSetupColumn(m_table->headers[i].name.c_str(), columns_base_flags | ImGuiTableColumnFlags_WidthStretch, 150.0f);
                }
                ImGui::TableSetupScrollFreeze(0, 1);
                ImGui::TableHeadersRow();
              }

              ImGuiListClipper clipper;
              clipper.Begin(m_table->numOfRows());
              while (clipper.Step()) {
                for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++) {
                  numOfDisplayedItems++;
                  Row* row = &m_table->rows[row_n];
                  ImGui::PushID(row->id);
                  ImGui::TableNextRow(ImGuiTableRowFlags_None, rowMinHeight);

                  for (int column_n = 0; column_n < m_table->numOfHeaders(); column_n++) {
                    ImGui::TableSetColumnIndex(column_n);
                    ImGui::TextUnformatted(row->data[column_n].c_str());
                  }
                  ImGui::PopID();
                }
              }

              ImGui::EndTable();
            } else {
              ImGui::TextColored(ImColor(255, 0, 0), "No headers found");
            }

            getRenderStats()->addStat("Num of displayed items", numOfDisplayedItems);
            getRenderStats()->addStat("Num of rows", m_table->numOfRows());
            getRenderStats()->addStat("Num of headers", m_table->numOfHeaders());
          }
        }
      }
      ImGui::EndChild();
    }

    void initialize() override {

    }

    void cleanUpOnClose() override {

    }

public:
    void setTable(const std::shared_ptr<Table> table) {
      m_table = table;
    }

private:
    std::shared_ptr<Table> m_table;
  };
}// namespace HummingBirdCore::Widgets
