//
// Created by Kasper de Bruin on 15/01/2024.
//

#include "SqlImportWindow.h"
#include <ImGuiFileDialog.h>
#include <ImGuiFileDialogConfig.h>
namespace HummingBirdCore::Sql {
  void SqlImportWindow::render() {

    // open Dialog Simple
    if (ImGui::Button("Open Files")) {
      ImGuiFileDialogFlags flags =
              ImGuiFileDialogFlags_Modal | ImGuiFileDialogFlags_HideColumnType;
      const char *filters = ".sql";

      IGFD::FileDialogConfig config;

      config.path = ".";
      config.countSelectionMax = 5;
      config.flags = flags;
      ImGuiFileDialog::Instance()->OpenDialog("ChooseSqlFileDlgKey",
                                              "Choose File(s)", filters, config);
    }

    // display imgui file dialog
    if (ImGuiFileDialog::Instance()->Display("ChooseSqlFileDlgKey")) {
      // action if OK
      if (ImGuiFileDialog::Instance()->IsOk()) {

        std::map<std::string, std::string> selection =
                ImGuiFileDialog::Instance()->GetSelection();// multiselection

        for (auto &file: selection) {
          HummingBirdCore::Utils::File f;
          // remove the extension
          // remove the name
          int index = file.second.find_last_of("/");
          int extensionIndex = file.second.find_last_of(".");
          std::string name =
                  file.second.substr(index + 1, extensionIndex - index - 1);
          std::string path = file.second.substr(0, index);

          f.setName(name);
          f.setPath(path);
          f.setExtension(".sql");

          m_files.push_back(f);
        }

        ImGuiFileDialog::Instance()->Close();
      }
    }

    ImGui::BeginChild("##SqlImportWindow", ImVec2(0, 0), true);
    ImGui::Columns(2, "##SqlImportWindowColumns", false);
    if (ImGui::BeginTable(
                "##SqlImportWindowTable", 3,
                ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable |
                        ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable |
                        ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter |
                        ImGuiTableFlags_BordersV | ImGuiTableFlags_NoBordersInBody |
                        ImGuiTableFlags_ScrollY)) {
      ImGui::TableSetupColumn("Path", ImGuiTableColumnFlags_WidthStretch);
      ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
      ImGui::TableSetupColumn("Extension", ImGuiTableColumnFlags_WidthStretch);
      ImGui::TableHeadersRow();
      int row = 0;
      for (auto &file: m_files) {
        ImGui::TableNextRow();
        ImGui::PushID(row);
        // Check if the row is selected

        ImGui::TableNextColumn();
        if (ImGui::Selectable(file.getPath().c_str(), m_selectedRow == row,
                              ImGuiSelectableFlags_SpanAllColumns)) {
          m_selectedRow = row;
        }
        ImGui::TableNextColumn();
        ImGui::Text("%s", file.getName().c_str());
        ImGui::TableNextColumn();
        ImGui::Text("%s", file.getExtension().c_str());
        ImGui::PopID();

        row++;
      }
      ImGui::EndTable();
    }

    ImGui::NextColumn();
    ImGui::BeginChild("##SqlImportWindowChild", ImVec2(0, 0), true);
    if (!m_sqlConnection.getIsConnected()) {
      ImGui::InputText("Server ip:", &m_inputServerName);
      ImGui::InputText("Username: ", &m_inputUserName);
      ImGui::InputText("Password: ", &m_inputPassword);

      if (ImGui::Button("Connect")) {
        if (!m_sqlConnection.connect(m_inputServerName, m_inputUserName,
                                     m_inputPassword)) {
          // error
          CORE_ERROR("Could not connect to the server: " + m_inputServerName);
        }
      }
    } else {
      ImGui::Text("Connected to: %s", m_inputServerName.c_str());
      ImGui::SameLine();
      if (ImGui::Button("Disconnect")) {
        m_sqlConnection.disconnect();
      }
      ImGui::SameLine();
      ImGui::Text("User: %s", m_inputUserName.c_str());
      ImGui::SameLine();
      ImGui::Text("Port: %i", m_sqlConnection.getPort());
      ImGui::SameLine();
      ImGui::Text("Database: %s",
                  m_sqlConnection.getCurrentDatabase().getName().c_str());
      ImGui::Separator();

      if (ImGui::BeginCombo(
                  "combo 1",
                  m_sqlConnection.getCurrentDatabase().getName().c_str())) {
        for (int n = 0; n < m_sqlConnection.getDatabases().size(); n++) {
          const bool is_selected =
                  (m_sqlConnection.getCurrentDatabase().getName() ==
                   m_sqlConnection.getDatabases()[n].getName());

          if (ImGui::Selectable(
                      m_sqlConnection.getDatabases()[n].getName().c_str(),
                      is_selected)) {
            m_sqlConnection.useDatabase(n);
          }

          // Set the initial focus when opening the combo (scrolling + keyboard
          // navigation focus)
          if (is_selected)
            ImGui::SetItemDefaultFocus();
        }

        if (ImGui::Button("new")) {
          // create popup modal
          ImGui::OpenPopup("CreateDatabase");
        }

        if (ImGui::BeginPopupModal("CreateDatabase")) {
          ImGui::InputText("Database name: ", &m_inputNewDatabaseName);
          if (ImGui::Button("Create")) {
            if (m_sqlConnection.createDatabase(m_inputNewDatabaseName)) {
              m_sqlConnection.useDatabase(m_sqlConnection.useDatabase(m_inputNewDatabaseName));
            }

            ImGui::CloseCurrentPopup();
          }
          ImGui::EndPopup();
        }

        ImGui::EndCombo();
      }
      if (m_selectedRow == -1) {
        ImGui::Text("No file selected");
      } else {
        ImGui::Text("%s", m_files[m_selectedRow].getName().c_str());


        if (ImGui::Button("Import")) {
          m_sqlConnection.importDatabase(m_files[m_selectedRow]);
        }
      }
    }

    ImGui::EndChild();
    ImGui::EndChild();
  }
}// namespace HummingBirdCore::Sql