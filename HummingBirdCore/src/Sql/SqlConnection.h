//
// Created by Kasper de Bruin on 06/12/2023.
//
#pragma once

#include <PCH/pch.h>

#include "UIWindows/Widget/DataViewer.h"

namespace HummingBirdCore::Sql {


  //TODO: Add a way to load this from a file
  struct SqlSettings {
    int maxTableSize = 1000;
    int maxRowSize = 10000;
  };

  enum SqlExportStatements{
    Insert,
    InsertWithColumns
  };

  struct Database {
public:
    Database(const std::string &name, const std::vector<std::shared_ptr<Widgets::Table>> tables) : name(name), tables(tables) {
    }

public:
    std::string getName() const {
      return name;
    }

    //TABLE FUNCTIONS
    void setTableData(const int tableIndex, const std::vector<Widgets::Header> &headers, const std::vector<Widgets::Row> &rows) {
      if (tableIndex < 0 || tableIndex >= tables.size()) {
        //        log("Table index out of range", spdlog::level::err);
        //TODO: Add log
        return;
      }
      tables[tableIndex]->setHeadersAndRows(headers, rows);
    }

    //GETTERS
    std::vector<std::shared_ptr<Widgets::Table>> getTables() const {
      return tables;
    }

    int getCurrentTableIndex() const {
      return currentTableIndex;
    }

    std::shared_ptr<Widgets::Table> getCurrentTable() const {
      return tables[currentTableIndex];
    }

    //SETTERS
    void setCurrentTableIndex(const int index) {
      if (index < 0 || index >= tables.size()) {
        //        log("Table index out of range", spdlog::level::err);
        //TODO: Add log
        return;
      }
      //      log("Setting current table index to: " + std::to_string(index), spdlog::level::info);
      //TODO: Add log
      currentTableIndex = index;
    }
    void setTables(const std::vector<std::shared_ptr<Widgets::Table>> &tables) {
      this->tables = tables;
    }

private:
    std::string name;
    std::vector<std::shared_ptr<Widgets::Table>> tables;
    int currentTableIndex = 0;
  };


  struct SqlExportSettings{
    std::string pathToMysqlDump = "";
    std::string exportPath = "";

    Database database;
    std::vector<std::string> tablesToExport = {};

    bool multipleRowsInserts = false;

  };

  class SqlConnection {
public:
    SqlConnection() : SqlConnection("", "", "", 3306, false) {
    }

    SqlConnection(const std::string &host, const std::string &user,
                  const std::string &password,
                  const unsigned int port = 3306,
                  const bool autoConnect = true);

    ~SqlConnection();

    bool connect();
    bool connect(const std::string &host, const std::string &user, const std::string &password,
                 const unsigned int port = 3306) {
      return connect(host, user, password, "", port);
    }
    bool connect(const std::string &host, const std::string &user,
                 const std::string &password, const std::string &database,
                 const unsigned int port = 3306);
    void disconnect();


    //Database functions
    bool useDatabase(const int databaseIndex);
    bool useDatabase(const std::string &database);

    bool useTable(const int tableIndex);
    bool useTable(const std::string &table);

    bool createDatabase(const std::string &databaseName);
    bool createTable(const std::string &tableName, const std::vector<Widgets::Header> &headers);

    bool importDatabase(Utils::File &file);

    //Server functions
    std::vector<Database> SVR_getDatabases() {
      if (!checkConnection()) {
        return {};
      }

      MYSQL_RES *res = mysql_list_dbs(m_connection, NULL);
      if (res == NULL) {
        //        log(mysql_error(m_connection), spdlog::level::err);
        //TODO: Add log
        return {};
      }

      std::vector<Database> result = {};
      MYSQL_ROW row;

      while ((row = mysql_fetch_row(res)) != NULL) {
        std::string name = row[0];
        std::vector<std::shared_ptr<Widgets::Table>> tables = {};
        Database db{name, tables};
        result.push_back(db);
      }
      mysql_free_result(res);
      for (Database &db: result) {
        db.setTables(SVR_getTables(db.getName()));
      }

      std::string logMsg = "Loaded " + std::to_string(result.size()) + " databases";
      //      log(logMsg, spdlog::level::info);
      //TODO: Add log
      return result;
    }
    std::vector<std::shared_ptr<Widgets::Table>> SVR_getTables(const std::string &database) {
      if (!checkConnection() || !checkDatabaseInput(database)) {
        return {};
      }

      int currentDbIndex = m_currentDatabaseIndex;

      int switched = mysql_select_db(m_connection, database.c_str());
      if (switched != 0) {
        //        log("Could not switch to database on the svr get tables: " + database, spdlog::level::err);
        //TODO: Add log
        return {};
      }


      int min = 0;
      int max = m_settings.maxTableSize;

      std::string query = "SELECT table_name FROM information_schema.tables WHERE table_schema = '" + database + "' LIMIT " + std::to_string(min) + ", " + std::to_string(max);
      int succes = mysql_query(m_connection, query.c_str());
      if (succes != 0) {
        //        log(mysql_error(m_connection), spdlog::level::err);
        //TODO: Add log
        return {};
      }

      MYSQL_RES *res = mysql_store_result(m_connection);
      if (res == NULL) {
        //        log(mysql_error(m_connection), spdlog::level::err);
        //TODO: Add log
        return {};
      }

      std::vector<std::shared_ptr<Widgets::Table>> result = {};

      MYSQL_ROW row;

      while ((row = mysql_fetch_row(res)) != NULL) {
        std::string name = row[0];
        std::vector<Widgets::Header> headers = {};
        std::vector<Widgets::Row> rows = {};
        std::shared_ptr<Widgets::Table> tablePtr = std::make_shared<Widgets::Table>(Widgets::Table{name, false, headers, rows, 0, "", 0, ""});
        result.push_back(tablePtr);
      }

      mysql_free_result(res);

      std::string logMsg = "Loaded " + std::to_string(result.size()) + " tables from database: " + database;
      //      log(logMsg, spdlog::level::info);
      //TODO: Add log
      return result;
    }
    std::vector<Widgets::Header> SVR_getHeaders(const std::string &database, const std::string &table) {
      if (!checkConnection() || !checkDatabaseInput(database) || !checkTableInput(table)) {
        return {};
      }

      int switched = mysql_select_db(m_connection, database.c_str());
      if (switched != 0) {
        //        log("Could not switch to database svr get header: " + database, spdlog::level::err);
        //TODO: Add log
        return {};
      }

      int succes = mysql_query(m_connection, ("SELECT * FROM " + table + " LIMIT 1").c_str());
      if (succes != 0) {
        //        log(mysql_error(m_connection), spdlog::level::err);
        //TODO: Add log
        return {};
      }

      MYSQL_RES *mysqlres = mysql_store_result(m_connection);
      if (mysqlres == NULL) {
        //        log(mysql_error(m_connection), spdlog::level::err);
        //TODO: Add log
        return {};
      }

      std::vector<Widgets::Header> result = {};
      MYSQL_FIELD *field;

      while ((field = mysql_fetch_field(mysqlres)) != NULL) {
        Widgets::Header header{field->name, field->type};
        result.push_back(header);
      }


      mysql_free_result(mysqlres);

      std::string logMsg = "Loaded " + std::to_string(result.size()) + " headers from table: " + table + " in database: " + database;
      //TODO: Add log
      return result;
    }

    std::vector<Widgets::Row> SVR_getAllRows(const std::string &database, const std::string &table) {

      //      if (!checkConnection() || !checkDatabaseInput(database) || !checkTableInput(table)) {
      //        return {};
      //      }

      int switched = mysql_select_db(m_connection, database.c_str());

      if (switched != 0) {
        //TODO: Add log
        return {};
      }
      int min = 0;
      int max = m_settings.maxRowSize;

      std::string query = "SELECT * FROM " + table + " LIMIT " + std::to_string(min) + ", " + std::to_string(max);
      int succes = mysql_query(m_connection, query.c_str());
      if (succes != 0) {
        return {};
      }

      MYSQL_RES *result = mysql_store_result(m_connection);
      if (result == NULL) {
        return {};
      }

      std::vector<Widgets::Row> rows;
      MYSQL_ROW mysql_row;
      unsigned int num_fields = mysql_num_fields(result);

      while ((mysql_row = mysql_fetch_row(result)) != NULL) {
        Widgets::Row row = Widgets::Row(0);
        if (mysql_row[0] != NULL) {
          row.id = atoi(mysql_row[0]);
        } else {
          row.id = -1;// or some other default value indicating an invalid ID
        }

        for (unsigned int i = 0; i < num_fields; i++) {
          if (mysql_row[i] != NULL) {
            std::string key = mysql_fetch_field_direct(result, i)->name;
            std::string value = mysql_row[i];
            row.setValue(key, value);
          } else {
            row.setValue(mysql_fetch_field_direct(result, i)->name, "NULL");
          }
        }
        rows.push_back(row);
      }


      mysql_free_result(result);

      std::string logMsg = "Loaded " + std::to_string(rows.size()) + " rows from table: " + table + " in database: " + database;
      //TODO: Add log
      return rows;
    }

    //TODO: Add a way to load this from a file
    void setLogger(const Ref<spdlog::logger> &logger) {
      m_logger = logger;
    }
    void setSettings(const SqlSettings &settings) {
      m_settings = SqlSettings{
              settings.maxTableSize,
              settings.maxRowSize};
    }


public:
    //Databases
    std::vector<Database> getDatabases() const { return m_databases; }
    Database getCurrentDatabase() const { return m_databases[getCurrentDatabaseIndex()]; }
    std::string getCurrentDatabaseName() const { return getCurrentDatabase().getName(); }
    int getCurrentDatabaseIndex() const { return m_currentDatabaseIndex; }
    int getPort() const { return m_port; }
    bool getIsConnected() const { return m_isConnected; }


private:
    bool checkConnection() {
      if (!m_isConnected) {
        //        log("Not connected to a database", spdlog::level::warn);
        //TODO: readd this
      }

      return getIsConnected();
    }

    bool checkDatabaseInput(const std::string &dbName) {
      //      if (dbName.empty()) {
      //        log("Database name is empty", spdlog::level::warn);
      //        return false;
      //      }
      //TODO: Add log
      return true;
    }

    bool checkTableInput(const std::string &tableName) {
      if (tableName.empty()) {
        //        log("Table name is empty", spdlog::level::warn);
        //TODO: Add log
        return false;
      }
      return true;
    }


private:
    std::string m_host;
    std::string m_user;
    std::string m_password;

    std::vector<Database> m_databases;
    int m_currentDatabaseIndex = 0;

    unsigned int m_port;

    bool m_isConnected = false;
    MYSQL *m_connection;

    Ref<spdlog::logger> m_logger = nullptr;

    SqlSettings m_settings;
  };
}// namespace HummingBirdCore::Sql
