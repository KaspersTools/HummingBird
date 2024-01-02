//
// Created by Kasper de Bruin on 06/12/2023.
//

#include "LoginManager.h"

//TODO: Implement login with database
bool HummingBirdCore::Security::LoginManager::s_isLoggedIn = true;

bool HummingBirdCore::Security::LoginManager::login(const char *username, const char *password) {
  CORE_INFO("Username: {}", username);
  std::string query = fmt::format("SELECT * FROM User WHERE name = '{}' AND password = '{}';", username, password);
  s_isLoggedIn = true;
  return false;
}

bool HummingBirdCore::Security::LoginManager::logout() {
  s_isLoggedIn = false;
  return true;
}

bool HummingBirdCore::Security::LoginManager::isLoggedIn() {
  return s_isLoggedIn;
}
