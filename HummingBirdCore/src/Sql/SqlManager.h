//
// Created by Kasper de Bruin on 07/12/2023.
//

#pragma once

#include <map>

#include "SqlConnection.h"
#include "../Log.h"

//Everything in this class is static
namespace HummingBirdCore::Sql {
  class SqlManager {
public:
    static bool Init(const std::string &host, const std::string &username, const std::string &password);

    static SqlConnection *GetMainConnection();

    static void Shutdown();


private:
    inline static bool s_initialized = false;
    inline static SqlConnection* s_mainConnection;

  };
}// namespace HummingBirdCore::Sql
