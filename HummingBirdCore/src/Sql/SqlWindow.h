//
// Created by Kasper de Bruin on 29/12/2023.
//

#pragma once

//#include "../UIWindows/UIWindow.h"
//#include <imgui_stdlib.h>
//
//#include "Logging/ImGuiLogSink.h"
//#include "SqlConnection.h"
//#include "CoreRef.h"

#include <memory>
#include <vector>

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

#include <imgui.h>
#include <imgui_stdlib.h>

#include "CoreRef.h"
#include "Logging/ImGuiLogSink.h"
#include "SqlConnection.h"
#include "../UIWindows/UIWindow.h"

namespace HummingBirdCore {
  namespace Sql {
    class SqlWindow : public HummingBirdCore::UIWindow {
  public:
      SqlWindow(ImGuiWindowFlags flags, const std::string &name, const bool autoEndFrame = true) : UIWindow(flags, name, autoEndFrame) {
        initializeLogSink();

        //Default values for the sql connection
        m_host = "127.0.0.1";
        m_user = "root";
        m_password = "NONE";
        m_database = "dsu_core";
        m_port = 3306;
      }

      ~SqlWindow() {
      }

      void render() override;

  private:
      void initializeLogSink() {
        if (m_logSink == nullptr) {
          m_logSink = std::make_shared<HummingBirdCore::Logging::ImGuiLogSink_mt>();
          m_logSink->set_pattern("[%T] [%l] %n: %v");

          m_logger = std::make_shared<spdlog::logger>("HummingBirdSQL " + getName(), m_logSink);
          m_logger->set_level(spdlog::level::trace);
          spdlog::register_logger(m_logger);

          m_connection.setLogger(m_logger);

        }else{
          m_logger->warn("Log sink already initialized");
        }
      }

  private:
      HummingBirdCore::Sql::SqlConnection m_connection;
      std::string m_host;
      std::string m_user;
      std::string m_password;
      std::string m_database;
      unsigned int m_port;

      Ref<spdlog::logger> m_logger = nullptr;
      std::shared_ptr <HummingBirdCore::Logging::ImGuiLogSink_mt> m_logSink = nullptr;
    };

  }// namespace Sql
}// namespace HummingBirdCore
