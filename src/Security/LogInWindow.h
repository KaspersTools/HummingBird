//
// Created by Kasper de Bruin on 06/12/2023.
//

#ifndef KBTOOLS_LOGINWINDOW_H
#define KBTOOLS_LOGINWINDOW_H
#include "../UIWindows/UIWindow.h"

namespace KBTools {
    namespace Security {

        class LogInWindow : public UIWindow {
        public:
            LogInWindow();

            ~LogInWindow();

            void Render() override;

        private:
            char m_username[32] = "username";
            char m_password[32] = "password";
            bool m_remember;
            bool m_failedLogin = false;
        };

    } // Security
} // KBTools
#endif //KBTOOLS_LOGINWINDOW_H
