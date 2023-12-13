//
// Created by Kasper de Bruin on 06/12/2023.
//

#pragma once

namespace HummingBirdCore::Security {
    class LoginManager {
    public:
        static bool Login(const char *username, const char *password);

        static bool Register(const char *username, const char *password);

        static bool Logout();

        static bool IsLoggedIn();

    private:
        static bool m_isLoggedIn;
    };
}
