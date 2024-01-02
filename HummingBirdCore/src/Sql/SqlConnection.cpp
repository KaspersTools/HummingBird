//
// Created by Kasper de Bruin on 06/12/2023.
//

#include "SqlConnection.h"

namespace HummingBirdCore::Sql {
  SqlConnection::SqlConnection(const std::string &host, const std::string &user,
                               const std::string &password,
                               const unsigned int port,
                               const bool autoConnect) : m_host(host), m_user(user), m_password(password), m_port(port) {
    m_connection = mysql_init(NULL);
    if (!m_connection) {
      log("Error initializing mysql", spdlog::level::err);
    } else {
      log("Initialized mysql", spdlog::level::info);
    }

    if (autoConnect) {
      connect();
    }
  }

  SqlConnection::~SqlConnection() {
    if (m_isConnected)
      disconnect();
  }

  bool SqlConnection::connect() {
    if (m_isConnected)
      disconnect();

    if (!mysql_real_connect(m_connection, m_host.c_str(), m_user.c_str(), m_password.c_str(),
                            "", m_port, NULL, 0)) {
      log("Error connecting to database", spdlog::level::err);
      log(mysql_error(m_connection), spdlog::level::err);
      return false;
    }

    m_isConnected = true;
    m_databases = SVR_getDatabases();

    log("Connected to database", spdlog::level::info);

    //Switch to first database
    if (m_databases.size() > 0) {
      useDatabase(0, 0);
    }

    return true;
  }

  bool SqlConnection::connect(const std::string &host, const std::string &user,
                              const std::string &password, const std::string &database,
                              const unsigned int port) {
    m_host = host;
    m_user = user;
    m_password = password;

    m_port = port;

    return connect();
  }

  void SqlConnection::disconnect() {
    if (!m_isConnected)
      return;

    mysql_close(m_connection);
    m_isConnected = false;
    log("Disconnected from database", spdlog::level::info);
  }

  bool SqlConnection::useDatabase(const int databaseIndex, int tableIndex) {
    if (!m_isConnected)
      return false;

    if (databaseIndex < 0 || databaseIndex >= m_databases.size()) {
      log("Database index out of range", spdlog::level::err);
      return false;
    }

    if(databaseIndex == m_currentDatabaseIndex){
      log("Database already selected only switching table", spdlog::level::warn);
      return useTable(tableIndex);
    }

    return useDatabase(m_databases[databaseIndex].getName(), tableIndex);
  }

  bool SqlConnection::useDatabase(const std::string &database, int tableIndex) {
    if (!m_isConnected)
      return false;

    int foundIndex = -1;
    for (std::size_t i = 0; i < m_databases.size(); ++i) {
      if (m_databases[i].getName() == database) {
        foundIndex = i;
        log("Found database: " + database, spdlog::level::info);
        break;
      }
    }
    if (foundIndex == -1) {
      log("Database: " + database + " not found", spdlog::level::err);
      return false;
    }
    const int savedDBIndex = m_currentDatabaseIndex;
    m_currentDatabaseIndex = foundIndex;
    if (!useTable(tableIndex)) {
      log("Failed to use table when switching database", spdlog::level::err);
      m_currentDatabaseIndex = savedDBIndex;
      return false;
    }
    log("Switched to database: " + database + " succesfully", spdlog::level::info);
    return true;
  }

  bool SqlConnection::useTable(const int tableIndex) {
    log("Trying to use table: " + std::to_string(tableIndex), spdlog::level::info);

    if (!m_isConnected)
      return false;

    if (tableIndex < 0 || tableIndex >= getCurrentDatabase().getTables().size()) {
      log("Table index out of range", spdlog::level::err);
      return false;
    }

    m_databases[m_currentDatabaseIndex].setCurrentTableIndex(tableIndex);
    const int currentTableIndex = m_databases[m_currentDatabaseIndex].getCurrentTableIndex();
    const std::string tableName = m_databases[m_currentDatabaseIndex].getTables()[currentTableIndex]->name;
    const bool isInitialized    = m_databases[m_currentDatabaseIndex].getTables()[currentTableIndex]->isInitialized;

    if (!isInitialized) {
      std::string logmsg = "Initializing table: " + tableName + " in database: " + getCurrentDatabaseName();
      log(logmsg, spdlog::level::info);

      std::vector<Widgets::Header> headers = SVR_getHeaders(getCurrentDatabaseName(), tableName);
      std::vector<Widgets::Row> rows = SVR_getAllRows(getCurrentDatabaseName(), tableName);

      m_databases[m_currentDatabaseIndex].setTableData(currentTableIndex, headers, rows);

      logmsg = "Initialized table: " + tableName + " in database: " + getCurrentDatabaseName();
      log(logmsg, spdlog::level::info);
    }

    std::string logMsg = "Switched to table: " + tableName + " in database: " + getCurrentDatabaseName();
    log(logMsg, spdlog::level::info);

    return true;
  }

  bool SqlConnection::useTable(const std::string &tableName) {
    if (!checkConnection() || !checkTableInput(tableName)) {
      return false;
    }

    for (std::size_t i = 0; i < getCurrentDatabase().getTables().size(); ++i) {
      if (getCurrentDatabase().getTables()[i]->name == tableName) {
        return useTable(i);
      }
    }
    return false;
  }

}// namespace HummingBirdCore::Sql
