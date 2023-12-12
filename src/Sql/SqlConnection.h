//
// Created by Kasper de Bruin on 06/12/2023.
//

#ifndef KBTOOLS_SQLCONNECTION_H
#define KBTOOLS_SQLCONNECTION_H

#include <iostream>
#include "../Log.h"

namespace HummingBird::Sql {
  class SqlConnection {
public:
    SqlConnection(const std::string &uri);
    ~SqlConnection();

    bool Connect();
    void Disconnect();
    bool IsConnected();
  };
}// namespace HummingBird::Sql


#endif//KBTOOLS_SQLCONNECTION_H
