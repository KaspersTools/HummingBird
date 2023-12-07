//
// Created by Kasper de Bruin on 06/12/2023.
//

#include "SqlConnection.h"
#include "../Log.h"


namespace HummingBird::Sql {

  SqlConnection::SqlConnection(const std::string &host, const std::string &username,
                               const std::string &password) : m_host(host), m_username(username),
                                                              m_password(password){
  }

  SqlConnection::~SqlConnection() {
    Disconnect();
  }

  bool SqlConnection::Connect() {
    if (m_isConnected) {
      return true;
    }

    conn = mysql_init(NULL);
    if (conn == NULL) {
      CORE_ERROR("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
      return false;
    }

    conn = mysql_real_connect(conn, m_host.c_str(), m_username.c_str(), m_password.c_str(), 0, 3309, 0,0);

    if (conn == NULL) {
      CORE_ERROR("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
      return false;
    }

    CORE_TRACE("Connected to mysql server on {}", m_host);

    m_isConnected = true;
    return true;
  }

  bool SqlConnection::Disconnect() {
    CORE_TRACE("Disconnecting from mysql server");
    if (!m_isConnected) {
      return true;
    }

    if(m_lastResult.GetResult() != nullptr){
      m_lastResult.FreeResult();
    }

    mysql_close(conn);

    return true;
  }

  bool SqlConnection::IsConnected() {
    return false;
  }

  SqlResult SqlConnection::Query(const std::string &query) {
    if (!m_isConnected) {
      CORE_WARN("Not connected to database");
      return {nullptr};
    }
    if(m_database == "NONE"){
      CORE_WARN("No database selected");
      return {nullptr};
    }
    int qr = mysql_query(conn, query.c_str());
    if (qr != 0) {
      CORE_ERROR("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
      return {nullptr};
    }

    SqlResult result = SqlResult(mysql_store_result(conn));
    return result;
  }

  void SqlConnection::SetDatabase(const std::string &database) {
    m_database = database;
    CORE_TRACE("Setting database to {}", database);
    mysql_select_db(conn, database.c_str());
  }
}// namespace HummingBird::Sql
