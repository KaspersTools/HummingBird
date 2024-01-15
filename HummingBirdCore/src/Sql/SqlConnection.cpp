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
      //log("Error initializing mysql", spdlog::level::err);
      //TODO: Add log
    } else {
      //log("Initialized mysql", spdlog::level::info);
      //TODO: Add log
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
      //log("Error connecting to database", spdlog::level::err);
      //log(mysql_error(m_connection), spdlog::level::err);
      //TODO: Add log

      return false;
    }

    m_isConnected = true;
    m_databases = SVR_getDatabases();

    //"Connected to database", spdlog::level::info);
    //TODO: Add log

    //Switch to first database
    if (m_databases.size() > 0) {
      useDatabase(0);
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
    //log("Disconnected from database", spdlog::level::info);
    //TODO: Add log
  }

  bool SqlConnection::useDatabase(const int databaseIndex) {
    if (!m_isConnected)
      return false;

    if (databaseIndex < 0 || databaseIndex >= m_databases.size()) {
      CORE_ERROR("Database index out of range");
      return false;
    }

    m_currentDatabaseIndex = databaseIndex;

    mysql_select_db(m_connection, getCurrentDatabaseName().c_str());

    CORE_INFO("Switched to database: " + getCurrentDatabaseName());
    return true;
  }

  bool SqlConnection::useDatabase(const std::string &database) {
    for (std::size_t i = 0; i < m_databases.size(); ++i) {
      if (m_databases[i].getName() == database) {
        return useDatabase(i);
      }
    }
  }

  bool SqlConnection::useTable(const int tableIndex) {
    if (!m_isConnected) {
      CORE_ERROR("No connection to server");
      return false;
    }

    if (tableIndex < 0 || tableIndex >= getCurrentDatabase().getTables().size()) {
      //log("Table index out of range", spdlog::level::err);
      CORE_ERROR("Table index out of range");
      return false;
    }

    m_databases[m_currentDatabaseIndex].setCurrentTableIndex(tableIndex);
    const int currentTableIndex = m_databases[m_currentDatabaseIndex].getCurrentTableIndex();
    const std::string tableName = m_databases[m_currentDatabaseIndex].getTables()[currentTableIndex]->name;
    const bool isInitialized = m_databases[m_currentDatabaseIndex].getTables()[currentTableIndex]->isInitialized;

    if (!isInitialized) {
      std::string logmsg = "Initializing table: " + tableName + " in database: " + getCurrentDatabaseName();
      //log(logmsg, spdlog::level::info);

      std::vector<Widgets::Header> headers = SVR_getHeaders(getCurrentDatabaseName(), tableName);
      std::vector<Widgets::Row> rows = SVR_getAllRows(getCurrentDatabaseName(), tableName);

      m_databases[m_currentDatabaseIndex].setTableData(currentTableIndex, headers, rows);

      logmsg = "Initialized table: " + tableName + " in database: " + getCurrentDatabaseName();
      //log(logmsg, spdlog::level::info);
    }

    std::string logMsg = "Switched to table: " + tableName + " in database: " + getCurrentDatabaseName();
    //log(logMsg, spdlog::level::info);

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

  bool SqlConnection::createDatabase(const std::string &databaseName) {
    if (!checkConnection()) {
      CORE_WARN("No connection to server");
      return false;
    }

    if (databaseName.empty()) {
      CORE_WARN("Database name is empty");
      return false;
    }

    std::string query = "CREATE DATABASE " + databaseName;
    if (mysql_query(m_connection, query.c_str())) {
      CORE_WARN("Could not create database: " + databaseName);
      return false;
    }

    m_databases = SVR_getDatabases();
    return true;
  }

  bool SqlConnection::importDatabase(Utils::File &file) {
    if (!checkConnection()) {
      CORE_ERROR("No connection to server");
      return false;
    }

    if (!Utils::FileUtils::fileExists(file)) {
      CORE_ERROR("File does not exist" + file.getFullPathWithExtension());
      return false;
    }

    if (file.getExtension() != ".sql") {
      CORE_WARN("File is not a sql file");
      return false;
    }

    CORE_TRACE("Importing into database : " + getCurrentDatabaseName());

    std::ifstream f(file.getFullPathWithExtension());
    std::string line;
    std::string query;

    if (f.is_open()) {
      while (getline(f, line)) {

        //skip line if it is a comment
        if (line.empty() || line[0] == '-' || line[0] == '/') {
          continue;
        }

        if (line.empty() || line.substr(0, 2) == "--") {
          continue;
        }
        query += line;
        if (line.back() == ';') {
          if (mysql_query(m_connection, query.c_str())) {
            CORE_ERROR("Could not import database: " + file.getName());
            CORE_ERROR(mysql_error(m_connection));
            CORE_ERROR(query);
          }
          query = "";
        }
      }
      f.close();
    } else {
      CORE_ERROR("Could not open file: " + file.getFullPathWithExtension());
    }

    m_databases = SVR_getDatabases();
    return true;
  }
}// namespace HummingBirdCore::Sql
