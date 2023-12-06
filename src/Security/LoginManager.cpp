//
// Created by Kasper de Bruin on 06/12/2023.
//

#include "LoginManager.h"
#include <cstring>

bool KBTools::Security::LoginManager::m_isLoggedIn = false;

bool KBTools::Security::LoginManager::Login(const char *username, const char *password) {
    //TODO: Implement login with database
    if(strcmp(username, "admin") != 0 || strcmp(password, "admin") != 0) {
        return false;
    }
    m_isLoggedIn = true;
    return true;
}

bool KBTools::Security::LoginManager::Register(const char *username, const char *password) {
    //TODO: Implement register with database
    return true;
}

bool KBTools::Security::LoginManager::Logout() {
    //TODO: Implement logout with database
    m_isLoggedIn = false;
    return true;
}

bool KBTools::Security::LoginManager::IsLoggedIn() {
    return m_isLoggedIn;
}


