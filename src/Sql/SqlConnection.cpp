`//
// Created by Kasper de Bruin on 06/12/2023.
//

#include "SqlConnection.h"
#include "../Log.h"
#include <mysql/mysql.h>


        namespace HummingBird::Sql {

  SqlConnection::SqlConnection(const std::string &host, const std::string &username,
                               const std::string &password) {
  }

  SqlConnection::~SqlConnection() {
    Disconnect();
  }

  bool SqlConnection::Connect() {
    if (m_isConnected) {
      if (!Disconnect()) {
        return false;
      }
    }

    conn = mysql_init(NULL);
    if (conn == NULL) {
      CORE_ERROR("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
      return false;
    }

    conn = mysql_real_connect(conn, "localhost", "root", "194910", "", 3306, NULL, 0);

    if (conn == NULL) {
      CORE_ERROR("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
      return false;
    }

    m_isConnected = true;
    return true;
  }

  bool SqlConnection::Connect(const std::string &host, const std::string &username,
                              const std::string &password) {
    m_host = host;
    m_username = username;
    m_password = password;
  }

  bool SqlConnection::Disconnect() {
    if (!m_isConnected) {
      return true;
    }

    mysql_close(conn);
    return true;
  }

  SqlResult SqlConnection::Query(const std::string &query) {
    if (!m_isConnected) {
      return {nullptr};
    }

    if (mysql_query(conn, query.c_str())) {
      CORE_ERROR("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
      return {nullptr};
    }

    return {mysql_store_result(conn)};
  }
}// Sql
