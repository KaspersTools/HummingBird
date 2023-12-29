//
// Created by Kasper de Bruin on 06/12/2023.
//
#pragma once

#include <iostream>
#include <mysql.h>

#include <memory>
#include <vector>

#pragma warning(push, 0)
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>
#pragma warning(pop)

#include "CoreRef.h"
#include "Log.h"

namespace HummingBirdCore::Sql {

  class SqlConnection {
public:
    SqlConnection() : SqlConnection("", "", "", "", 3306, false) {
      CORE_TRACE("Default constructor called");
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
        m_logger->log(level, message);
      }
    }

private:
    std::string m_host;
    std::string m_user;
    std::string m_password;
    std::string m_database;
    unsigned int m_port;

    bool m_isConnected = false;
    MYSQL *m_connection;

    Ref<spdlog::logger> m_logger = nullptr;
  };
}// namespace HummingBirdCore::Sql
