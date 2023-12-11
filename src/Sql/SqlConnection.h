//
// Created by Kasper de Bruin on 06/12/2023.
//

#ifndef KBTOOLS_SQLCONNECTION_H
#define KBTOOLS_SQLCONNECTION_H

#include <iostream>
#include <mysql.h>

#include "../Log.h"

namespace HummingBird::Sql {

  struct SqlResult {
public:
    SqlResult(MYSQL_RES *result) : m_result(result) {}

    MYSQL_ROW FetchRow()                     const { CORE_TRACE("Fetch row"); return mysql_fetch_row(m_result); }
    unsigned int GetNumFields()              const { CORE_TRACE("Get Num Fields"); return mysql_num_fields(m_result); }
    unsigned long *GetLengths()              const { CORE_TRACE("Get Lengths"); return mysql_fetch_lengths(m_result); }
    unsigned long GetNumRows()               const { CORE_TRACE("Get Num Rows"); return mysql_num_rows(m_result); }
    MYSQL_FIELD *GetFields()                 const { CORE_TRACE("Get fields" ); return mysql_fetch_fields(m_result); }
    MYSQL_FIELD *GetField(unsigned int i)    const { CORE_TRACE("Get Field {}", i); return mysql_fetch_field_direct(m_result, i); }
    MYSQL_FIELD *GetField(const char *name)  const { CORE_TRACE("Get Field {}", name); return mysql_fetch_field(m_result); }
    MYSQL_RES *GetResult()                   const { CORE_TRACE("Get Result"); return m_result; }
    bool IsEmpty()                           const { CORE_TRACE("Get Is Empty"); return mysql_num_rows(m_result) == 0; }
    bool IsNotEmpty()                        const { CORE_TRACE("Get Is Not Empty"); return mysql_num_rows(m_result) > 0; }
    bool WasSuccessful()                     const { CORE_TRACE("Get Was Succesfull"); return m_result != nullptr; }
    void FreeResult()                        const { CORE_TRACE("Free last result"); mysql_free_result(m_result); }

private:
    MYSQL_RES *m_result = nullptr;
  };


  class SqlConnection {
public:
    SqlConnection(const std::string &host, const std::string &username, const std::string &password);

    ~SqlConnection();

    bool Connect();
    bool Disconnect();
    void SetDatabase(const std::string &database);
    MYSQL *GetConnection() { return conn; }
    SqlResult Query(const std::string &query);
    bool IsConnected();

    const SqlResult GetLastResult() const { return m_lastResult; }

private:
    MYSQL *conn;
    bool m_isConnected = false;
    const std::string m_host;
    const std::string m_username;
    const std::string m_password;
    SqlResult m_lastResult = nullptr;
    std::string m_database = "NONE";
  };
}// namespace HummingBird::Sql


#endif//KBTOOLS_SQLCONNECTION_H
