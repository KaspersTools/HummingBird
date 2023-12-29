//
// Created by Kasper de Bruin on 06/12/2023.
//
#pragma once

#include <iostream>
#include <mysql.h>

#include <chrono>
#include <functional>
#include <map>
#include <memory>
#include <vector>

#pragma warning(push, 0)
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>
#pragma warning(pop)

#include "CoreRef.h"
#include "Log.h"

namespace HummingBirdCore::Sql {
  struct QueryResult {
    bool success;
    std::string error;
    std::vector<std::vector<std::string>> data;
    std::string source;
    std::size_t rowCount;
    std::vector<std::string> columnNames;
    std::size_t fieldCount;
    std::vector<unsigned int> fieldTypes;
    std::map<std::string, std::size_t> fieldIndexMap;

    QueryResult() : success(false), error(""), data({}), source(""), rowCount(0), columnNames({}), fieldCount(0), fieldTypes({}), fieldIndexMap({}) {
    }

    QueryResult(bool success, std::string error, std::vector<std::vector<std::string>> data,
                std::string source, std::size_t rowCount, std::vector<std::string> columnNames,
                std::size_t fieldCount, std::vector<unsigned int> fieldTypes,
                std::map<std::string, std::size_t> fieldIndexMap)
        : success(success), error(error), data(data), source(source), rowCount(rowCount),
          columnNames(columnNames), fieldCount(fieldCount), fieldTypes(fieldTypes), fieldIndexMap(fieldIndexMap) {
    }
  };

  enum class QueryType {
    INSERT,
    SELECT,
    UPDATE,
    DELETE
  };


  class SqlConnection {
public:
    SqlConnection() : SqlConnection("", "", "", "", 3306, false) {
    }

    SqlConnection(const std::string &host, const std::string &user,
                  const std::string &password, const std::string &database,
                  const unsigned int port = 3306,
                  const bool autoConnect = true);

    ~SqlConnection();

    bool connect();
    bool connect(const std::string &host, const std::string &user,
                 const std::string &password, const std::string &database,
                 const unsigned int port = 3306);
    void disconnect();
    bool isConnected();

    const QueryResult query(const std::string &query);
    QueryResult getAllTables();

    QueryResult getLastTableQuery() const {
      return m_allTables;
    }

    QueryResult getAllDatabaseNames();

    QueryResult getLastDatabaseNames() const {
      return m_allDatabaseNames;
    }

    void setLogger(Ref<spdlog::logger> logger) {
      m_logger = logger;
    }

private:
    //Custom logger so we can log to the ImGuiLogSink
    void log(std::string message, spdlog::level::level_enum level = spdlog::level::trace) {
      if (m_logger == nullptr) {
        CORE_ERROR("Logger is null set the logger before using the log function");
        CORE_WARN("Message: {0}, we will now just log to the core logger", message);
      } else {
        //        CORE_LOG(message, level);
        m_logger->log(level, message);
        HummingBirdCore::Log::GetCoreLogger()->log(level, message);
      }
    }
//
//    void processQueryResults(MYSQL_RES *result, std::function<void(MYSQL_RES *)> resultProcessor);
//    std::string constructQuery(const std::string &table, const std::map<std::string, std::string> &data, const std::string &type);


private:
    std::string m_host;
    std::string m_user;
    std::string m_password;
    std::string m_database;
    unsigned int m_port;

    bool m_isConnected = false;
    MYSQL *m_connection;

    Ref<spdlog::logger> m_logger = nullptr;

    QueryResult m_allTables = QueryResult();

    QueryResult m_allDatabaseNames = {};
  };
}// namespace HummingBirdCore::Sql
