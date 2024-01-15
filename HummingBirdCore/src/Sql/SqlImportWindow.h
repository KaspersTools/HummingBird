//
// Created by Kasper de Bruin on 15/01/2024.
//

#ifndef HUMMINGBIRD_SQLIMPORTWINDOW_H
#define HUMMINGBIRD_SQLIMPORTWINDOW_H

#include <PCH/pch.h>
#include <Sql/SqlConnection.h>
#include <UIWindows/UIWindow.h>

namespace HummingBirdCore::Sql {
  class SqlImportWindow : public HummingBirdCore::UIWindow {
public:
    SqlImportWindow(const std::string &name) : UIWindow(ImGuiWindowFlags_None, name) {
    }

    ~SqlImportWindow() {
    }

    void render() override;

private:
    std::vector<HummingBirdCore::Utils::File> m_files = {};
    int m_selectedRow = -1;// Variable to track the selected row

    HummingBirdCore::Sql::SqlConnection m_sqlConnection;

    std::string m_inputServerName = "";
    std::string m_inputUserName = "";
    std::string m_inputPassword = "";

    std::string m_inputNewDatabaseName = "";
  };
}// namespace HummingBirdCore::Sql
#endif// HUMMINGBIRD_SQLIMPORTWINDOW_H
