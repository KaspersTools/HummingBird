//
// Created by Kasper de Bruin on 06/12/2023.
//

#include "LoginManager.h"
#include "../Application.h"
#include <cstring>

bool HummingBirdCore::Security::LoginManager::s_isLoggedIn = false;

bool HummingBirdCore::Security::LoginManager::login(const char *username, const char *password) {
  //TODO: Implement login with database
  //    if(strcmp(username, "admin") != 0 || strcmp(password, "admin") != 0) {
  //        return false;
  //    }
  //    m_isLoggedIn = true;
  //Check if the username and password are correct
  //If they are correct, set m_isLoggedIn to true
  //If they are not correct, return false
  CORE_INFO("Username: {}", username);


  std::string query = fmt::format("SELECT * FROM User WHERE name = '{}' AND password = '{}';", username, password);
  //TODO: Implement query
  //    HummingBirdCore::Sql::SqlResult result = HummingBirdCore::Sql::SqlManager::GetMainConnection()->Query(query);
  //    if(result.WasSuccessful()){
  //        if(result.GetResult()->row_count == 1) {
  //            m_isLoggedIn = true;
  //            return true;
  //        }
  //      }
  s_isLoggedIn = true;
  return false;
}

//bool HummingBirdCore::Security::LoginManager::register(const char *username, const char *password) {
//  //TODO: Implement register with database
//  return true;
//}

bool HummingBirdCore::Security::LoginManager::logout() {
  //TODO: Implement logout with database
  s_isLoggedIn = false;
  return true;
}

bool HummingBirdCore::Security::LoginManager::isLoggedIn() {
  return s_isLoggedIn;
}
