//
// Created by Kasper de Bruin on 07/12/2023.
//

#ifndef KBTOOLS_SQLEXAMPLE_H
#define KBTOOLS_SQLEXAMPLE_H

#include "SqlConnection.h"

namespace HummingBird::Sql {
    class SqlExample {
    public:
        SqlExample(){
          SqlConnection conn = SqlConnection("host", "username", "passwd");
          if(conn.Connect()){

            conn.SetDatabase("KCC_UserManagement");
            SqlResult result = conn.Query("SELECT * FROM `userrole`;");

            if(result.WasSuccessful()){
              std::cout << "Query was successful" << std::endl;
              MYSQL_ROW row;
              while((row = result.FetchRow()) != NULL){
                std::cout << row[0] << std::endl;
              }
            } else {
              std::cout << "Query was not successful" << std::endl;
            }

          } else {
            std::cout << "Not Connected" << std::endl;
          }
        }
    };
}

#endif//KBTOOLS_SQLEXAMPLE_H
