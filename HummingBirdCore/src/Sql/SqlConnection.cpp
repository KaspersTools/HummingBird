//
// Created by Kasper de Bruin on 06/12/2023.
//

#include "SqlConnection.h"
#include "../Log.h"
#include <chrono>


namespace HummingBirdCore::Sql {
  //TODO: Implement this class
  SqlConnection::SqlConnection(const std::string &host, const std::string &user,
                               const std::string &password, const std::string &database,
                               const unsigned int port,
                               const bool autoConnect) : m_host(host), m_user(user), m_password(password), m_database(database), m_port(port) {
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
                            m_database.c_str(), m_port, NULL, 0)) {
      log("Error connecting to database", spdlog::level::err);
      log(mysql_error(m_connection), spdlog::level::err);
      CORE_ERROR("Error connecting to database {0}", mysql_error(m_connection));
      return false;
    }

    m_isConnected = true;
    log("Connected to database", spdlog::level::info);
    return true;
  }

  bool SqlConnection::connect(const std::string &host, const std::string &user,
                              const std::string &password, const std::string &database,
                              const unsigned int port) {
    m_host = host;
    m_user = user;
    m_password = password;
    m_database = database;
    m_port = port;

    return connect();
  }

  void SqlConnection::disconnect() {
    if (!m_isConnected)
      return;

    mysql_close(m_connection);
    m_isConnected = false;
  }


  bool SqlConnection::isConnected() {
    return m_isConnected;
  }

  const QueryResult SqlConnection::query(const std::string &query) {
    QueryResult result;
    auto start = std::chrono::high_resolution_clock::now();
    int q = mysql_query(m_connection, query.c_str());

    if (q) {
      result.success = false;
      result.error = mysql_error(m_connection);
      std::string error = "Error executing query: " + result.error;
      log(error, spdlog::level::err);
      return result;
    }

    MYSQL_RES *res = mysql_store_result(m_connection);
    MYSQL_FIELD *field;
    MYSQL_ROW row;

    if (res) {
      result.fieldCount = mysql_num_fields(res);

      // Fetch field types and names
      while ((field = mysql_fetch_field(res)) != NULL) {
        result.fieldTypes.push_back(field->type);
        result.fieldIndexMap[field->name] = mysql_field_tell(res);
        result.columnNames.push_back(field->name);
      }

      // Fetch rows
      while ((row = mysql_fetch_row(res)) != NULL) {
        std::vector<std::string> rowData;
        unsigned long* lengths = mysql_fetch_lengths(res);
        for (unsigned int i = 0; i < result.fieldCount; ++i) {
          rowData.push_back(row[i] ? std::string(row[i], lengths[i]) : "NULL");
        }
        result.data.push_back(rowData);
      }

      mysql_free_result(res); // Free the result set
    } else {
      if (mysql_field_count(m_connection) == 0) {
        result.rowCount = mysql_affected_rows(m_connection);
      } else {
        result.success = false;
        result.error = mysql_error(m_connection);
        std::string error = "Error fetching result set: " + result.error;
        log(error, spdlog::level::err);
        return result;
      }
    }

    std::string success = std::string("Executed query: ") + query + " successfully in " +
                          std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(
                                                 std::chrono::high_resolution_clock::now() - start).count()) + "ms";
    log(success, spdlog::level::info);

    result.success = true;
    result.error = "";
    result.source = query;
    return result;
  }

}// namespace HummingBirdCore::Sql
