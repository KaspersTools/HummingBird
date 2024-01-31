//
// Created by Kasper de Bruin on 31/01/2024.
//

#ifndef HUMMINGBIRD_SQLWINDOW_H
#define HUMMINGBIRD_SQLWINDOW_H

#include <UIWindows/UIWindow.h>
#include <Hummingbird_SQL/Connection.h>

namespace HummingBirdCore {
  class SqlWindow : public UIWindow {
  public:
    SqlWindow(std::string name) : UIWindow(name) {
    }

    void render() override;
private:
    std::string m_inputServerName = "127.0.0.1";
    std::string m_inputUserName = "root";
    std::string m_inputPassword = "";

    HummingBird::Sql::Connection m_connection;
  };
}

#endif//HUMMINGBIRD_SQLWINDOW_H
