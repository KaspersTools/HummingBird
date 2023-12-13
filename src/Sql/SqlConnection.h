//
// Created by Kasper de Bruin on 06/12/2023.
//
#pragma once

#include <iostream>
#include "../Log.h"

namespace HummingBirdCore::Sql {
  class SqlConnection {
public:
    SqlConnection(const std::string &uri);
    ~SqlConnection();

    bool Connect();
    void Disconnect();
    bool IsConnected();
  };
}// namespace HummingBirdCore::Sql
