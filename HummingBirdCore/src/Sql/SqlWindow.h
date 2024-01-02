//
// Created by Kasper de Bruin on 29/12/2023.
//

#pragma once
#include <PCH/pch.h>

#include "UIWindows/UIWindow.h"
#include "UIWindows/Widget/DataViewer.h"

namespace HummingBirdCore {
  namespace Sql {
    class SqlWindow : public HummingBirdCore::UIWindow {
  public:
      SqlWindow(const std::string &name) : SqlWindow(ImGuiWindowFlags_None, name) {
      }
      SqlWindow(ImGuiWindowFlags flags, const std::string &name, const bool autoEndFrame = true) : UIWindow(flags, name, autoEndFrame) {
        initialize();
      }

      ~SqlWindow() {
      }

      void render() override;
      void initialize() override{
        initializeLogSink();

        //Default values for the sql connection
        m_inputhost = "127.0.0.1";
        m_inputuser = "root";
        m_inputpassword = "Vuur01-";
        m_port = 3306;

        m_dataViewer = std::make_shared<HummingBirdCore::Widgets::DataViewer>(getName() + " Data Viewer");

        getRenderStats()->addChildWindow(c_renderStatsMainName);
        getRenderStats()->addChildWindow(c_renderStatsLeftName);
        getRenderStats()->addChildWindow(c_renderStatsRightName);
        getRenderStats()->addChildWindow(m_dataViewer->getRenderStats());
      }

      void cleanUpOnClose() override{
        m_dataViewer->cleanUpOnClose();
        m_dataViewer = nullptr;

        m_logSink = nullptr;
        m_logger = nullptr;

        spdlog::drop("HummingBirdSQL " + getName());

        m_connection.disconnect();
      }
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
      //default flags
      const ImGuiWindowFlags c_leftWindowFlags = ImGuiWindowFlags_HorizontalScrollbar;
      const ImGuiChildFlags c_leftChildFlags = ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX;
      const ImGuiTreeNodeFlags c_baseTreeNodeFlagsLeft = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
      const ImGuiSelectableFlags c_selectableFlags = ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_SpanAllColumns;

      SqlConnection m_connection;

      std::string m_inputhost;
      std::string m_inputuser;
      std::string m_inputpassword;

      unsigned int m_port;

      Ref<spdlog::logger> m_logger = nullptr;
      std::shared_ptr<HummingBirdCore::Logging::ImGuiLogSink_mt> m_logSink = nullptr;
      std::shared_ptr<HummingBirdCore::Widgets::DataViewer> m_dataViewer = nullptr;

      const std::string c_renderStatsMainName = getName()  + " Data Main Render Stats : ";
      const std::string c_renderStatsLeftName = getName()  + " Left Render Stats      : ";
      const std::string c_renderStatsRightName = getName() + " Right Render Stats     : ";
    };

  }// namespace Sql
}// namespace HummingBirdCore
