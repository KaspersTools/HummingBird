//
// Created by Kasper de Bruin on 06/12/2023.
//

#include "SqlConnection.h"
#include "../Log.h"


namespace HummingBirdCore::Sql {
  //TODO: Implement this class
  SqlConnection::SqlConnection(const std::string& host, const std::string& user,
                               const std::string& password, const std::string& database,
                               const unsigned int port,
                               const bool autoConnect
  ) :
     m_host(host), m_user(user), m_password(password), m_database(database), m_port(port)
  {
    m_connection = mysql_init(NULL);
    if (!m_connection) {
      log("Error initializing mysql", spdlog::level::err);
    }
    else{
      log("Initialized mysql", spdlog::level::info);
    }

    if (autoConnect) {
      connect();
    }

  }

  SqlConnection::~SqlConnection() {
    if(m_isConnected)
      disconnect();
  }

   bool SqlConnection::connect() {
    if(m_isConnected)
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

  bool SqlConnection::connect(const std::string& host, const std::string& user,
                              const std::string& password, const std::string& database,
                              const unsigned int port) {
      m_host = host;
      m_user = user;
      m_password = password;
      m_database = database;
      m_port = port;

      return connect();
  }

  void SqlConnection::disconnect() {
    if(!m_isConnected)
      return;

    mysql_close(m_connection);
    m_isConnected = false;
  }


  bool SqlConnection::isConnected() {
    return m_isConnected;
  }
}// namespace HummingBirdCore::Sql
