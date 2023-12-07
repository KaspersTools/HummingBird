//
// Created by Kasper de Bruin on 06/12/2023.
//

#ifndef KBTOOLS_SQLCONNECTION_H
#define KBTOOLS_SQLCONNECTION_H

#include <iostream>
#include <mysql/mysql.h>


namespace HummingBird::Sql {

  struct SqlResult {
public:
    SqlResult(MYSQL_RES *result) : m_result(result) {}

    ~SqlResult() { mysql_free_result(m_result); }
    MYSQL_ROW FetchRow() { return mysql_fetch_row(m_result); }
    unsigned int GetNumFields() { return mysql_num_fields(m_result); }
    unsigned long *GetLengths() { return mysql_fetch_lengths(m_result); }
    unsigned long GetNumRows() { return mysql_num_rows(m_result); }
    MYSQL_FIELD *GetFields() { return mysql_fetch_fields(m_result); }
    MYSQL_FIELD *GetField(unsigned int i) { return mysql_fetch_field_direct(m_result, i); }
    MYSQL_FIELD *GetField(const char *name) { return mysql_fetch_field(m_result); }
    MYSQL_RES *GetResult() { return m_result; }
    bool IsEmpty() { return mysql_num_rows(m_result) == 0; }
    bool IsNotEmpty() { return mysql_num_rows(m_result) > 0; }
    bool WasSuccessful() { return m_result != nullptr; }

private:
    MYSQL_RES *m_result;
  };


  class SqlConnection {
public:
    SqlConnection(const std::string &host, const std::string &username, const std::string &password);

    ~SqlConnection();

    bool Connect();
    bool Connect(const std::string &host, const std::string &username, const std::string &password);

    bool Disconnect();

    MYSQL *GetConnection() { return conn; }

    SqlResult Query(const std::string &query);

    bool IsConnected();

private:
    MYSQL *conn;
    bool m_isConnected = false;
    std::string m_host;
    std::string m_username;
    std::string m_password;
  };// SqlConnection
}// namespace HummingBird::Sql
// HummingBird

#endif//KBTOOLS_SQLCONNECTION_H
