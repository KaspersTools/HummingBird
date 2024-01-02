//
// Created by Kasper de Bruin on 06/12/2023.
//

#pragma once

#include <PCH/pch.h>

namespace HummingBirdCore::Security {
  class LoginManager {
public:
    static bool login(const char *username, const char *password);

//    static bool register(const char *username, const char *password);

    static bool logout();

    static bool isLoggedIn();

private:
    static bool s_isLoggedIn;
  };
}// namespace HummingBirdCore::Security
