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
        //Default values for the sql connection
        m_inputhost = "127.0.0.1";
        m_inputuser = "root";
        m_inputpassword = "pwd";
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

        m_connection.disconnect();
      }
  private:
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

      int m_port;

      std::shared_ptr<HummingBirdCore::Widgets::DataViewer> m_dataViewer = nullptr;

      const std::string c_renderStatsMainName = getName()  + " Data Main Render Stats : ";
      const std::string c_renderStatsLeftName = getName()  + " Left Render Stats      : ";
      const std::string c_renderStatsRightName = getName() + " Right Render Stats     : ";
    };

  }// namespace Sql
}// namespace HummingBirdCore
