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

#include <chrono>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#pragma warning(push, 0)
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>
#pragma warning(pop)

#include <imgui.h>
#include <imgui_stdlib.h>

#include "../UIWindows/UIWindow.h"
#include "CoreRef.h"
#include "Logging/ImGuiLogSink.h"
#include "SqlConnection.h"

namespace HummingBirdCore {



  namespace Sql {
    struct SqlWindowSettings{
      const int c_defaultMaxTableSize = 100;
      const int c_defaultMaxDatabaseSize = 100;
      const int c_defaultMaxDataSize = 100;
    };

    class SqlWindow : public HummingBirdCore::UIWindow {
  public:
      SqlWindow(const std::string& name) : SqlWindow(ImGuiWindowFlags_None, name) {

      }

      SqlWindow(ImGuiWindowFlags flags, const std::string &name, const bool autoEndFrame = true) : UIWindow(flags, name, autoEndFrame) {
        initializeLogSink();

        //Default values for the sql connection
        m_host = "127.0.0.1";
        m_user = "root";
        m_password = "NONE";
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
        } else {
          m_logger->warn("Log sink already initialized");
        }
      }

      void renderQueryTab();
      void renderTablesTab();
      void renderDatabasesTab();
      void renderDataTab();

  private:
      //Window flags
      const ImGuiWindowFlags c_leftWindowFlags = ImGuiWindowFlags_HorizontalScrollbar;
      const ImGuiChildFlags c_leftChildFlags   = ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX;

      const ImGuiWindowFlags c_rightWindowFlags = ImGuiWindowFlags_HorizontalScrollbar ;
      const ImGuiChildFlags c_rightChildFlags = ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeX;

      //Tree node flags
      const ImGuiTreeNodeFlags c_baseTreeNodeFlagsLeft = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
      const ImGuiSelectableFlags c_selectableFlags = ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_SpanAllColumns;

      HummingBirdCore::Sql::SqlConnection m_connection;
      std::string m_host;
      std::string m_user;
      std::string m_password;

      unsigned int m_port;

      std::string m_queryInput = "";

      Ref<spdlog::logger> m_logger = nullptr;
      std::shared_ptr<HummingBirdCore::Logging::ImGuiLogSink_mt> m_logSink = nullptr;

      SqlWindowSettings m_settings;
    };

  }// namespace Sql
}// namespace HummingBirdCore
