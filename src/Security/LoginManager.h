//
// Created by Kasper de Bruin on 06/12/2023.
//

#ifndef KBTOOLS_LOGINMANAGER_H
#define KBTOOLS_LOGINMANAGER_H

namespace KBTools::Security {
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


#endif //KBTOOLS_LOGINMANAGER_H
