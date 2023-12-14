//
// Created by Kasper de Bruin on 06/12/2023.
//

#pragma once
#include "../UIWindows/UIWindow.h"

namespace HummingBirdCore {
    namespace Security {

        class LogInWindow : public UIWindow {
        public:
            LogInWindow();

            ~LogInWindow();

            void Render() override;

        private:
            char m_username[32] = "admin";
            char m_password[32] = "admin";
            bool m_remember;
            bool m_failedLogin = false;
        };

    } // Security
} // KBTools
