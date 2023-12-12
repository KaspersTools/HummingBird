//
// Created by Kasper de Bruin on 06/12/2023.
//

#include "SqlConnection.h"
#include "../Log.h"


namespace HummingBird::Sql {
  //TODO: Implement this class
  SqlConnection::SqlConnection(const std::string &uri) {

  }

  SqlConnection::~SqlConnection() {

  }
  bool SqlConnection::Connect() {
    return true;
  }
  void SqlConnection::Disconnect() {

  }

  bool SqlConnection::IsConnected() {
    return true;
  }
}// namespace HummingBird::Sql
