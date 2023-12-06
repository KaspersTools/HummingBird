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
            char m_username[256];
            char m_password[256];
            bool m_remember;
        };

    } // Security
} // KBTools
#endif //KBTOOLS_LOGINWINDOW_H
