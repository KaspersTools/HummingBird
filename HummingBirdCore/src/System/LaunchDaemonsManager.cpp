//
// Created by Kasper de Bruin on 08/01/2024.
//

#include "LaunchDaemonsManager.h"

namespace HummingBirdCore {
  namespace System {
    void LaunchDaemonsManager::render() {

      //Left resizeable panel
      {
        ImGui::BeginChild("LaunchDaemons", ImVec2(ImGui::GetContentRegionAvail().x * 0.2f, 0), c_leftChildFlags, c_leftWindowFlags);
        ImGui::Text("LaunchDaemons");
        ImGui::Separator();

        //tab
        ImGuiTabBarFlags tabBarFlags = ImGuiTabBarFlags_Reorderable;
        tabBarFlags |= ImGuiTabBarFlags_FittingPolicyDefault_;
        tabBarFlags &= ~(ImGuiTabBarFlags_FittingPolicyMask_ ^ ImGuiTabBarFlags_FittingPolicyScroll);
        //
        //        if (ImGui::BeginTabBar("LaunchDaemonsTabBar"), tabBarFlags) {
        //          if (ImGui::BeginTabItem("User Agent")) {
        //            int index = 0;
        //            for (auto &daemon: m_userAgent) {
        //              if (ImGui::Selectable(daemon.getFileName().c_str(), m_selectedUserAgentDaemon == index)) {
        //                m_selectedUserAgentDaemon = index;
        //              }
        //              index++;
        //            }
        //            ImGui::EndTabItem();
        //          }
        //          if (ImGui::BeginTabItem("Global Agent")) {
        //            int index = 0;
        //            for (auto &daemon: m_userAgent) {
        //              if (ImGui::Selectable(daemon.getFileName().c_str(), m_selectedUserAgentDaemon == index)) {
        //                m_selectedUserAgentDaemon = index;
        //              }
        //              index++;
        //            }
        //            ImGui::EndTabItem();
        //          }
        //          if (ImGui::BeginTabItem("Global Daemon")) {
        //            int index = 0;
        //            for (auto &daemon: m_userAgent) {
        //              if (ImGui::Selectable(daemon.getFileName().c_str(), m_selectedUserAgentDaemon == index)) {
        //                m_selectedUserAgentDaemon = index;
        //              }
        //              index++;
        //            }
        //            ImGui::EndTabItem();
        //          }
        //          if (ImGui::BeginTabItem("System Agent")) {
        //            int index = 0;
        //            for (auto &daemon: m_userAgent) {
        //              if (ImGui::Selectable(daemon.getFileName().c_str(), m_selectedUserAgentDaemon == index)) {
        //                m_selectedUserAgentDaemon = index;
        //              }
        //              index++;
        //            }
        //            ImGui::EndTabItem();
        //          }
        //          if (ImGui::BeginTabItem("System Daemon")) {
        //            int index = 0;
        //            for (auto &daemon: m_userAgent) {
        //              if (ImGui::Selectable(daemon.getFileName().c_str(), m_selectedUserAgentDaemon == index)) {
        //                m_selectedUserAgentDaemon = index;
        //              }
        //              index++;
        //            }
        //            ImGui::EndTabItem();
        //          }
        //
        //          ImGui::EndTabBar();
        //        }


        static ImVector<int> active_tabs;
        static int next_tab_id = 0;
        if (next_tab_id == 0)// Initialize with some default tabs
          for (int i = 0; i < 5; i++)
            active_tabs.push_back(next_tab_id++);


        // Expose some other flags which are useful to showcase how they interact with Leading/Trailing tabs
        static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable
                                                | ImGuiTabBarFlags_FittingPolicyScroll
                                                | ImGuiTabBarFlags_TabListPopupButton;

        if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags)) {
          renderTab("User Agent", m_userAgent);
          renderTab("Global Agent", m_globalAgent);
          renderTab("Global Daemon", m_globalDaemon);
          renderTab("System Agent", m_systemAgent);
          renderTab("System Daemon", m_systemDaemon);
          ImGui::EndTabBar();
        }


        ImGui::Separator();
        ImGui::EndChild();
      }

      //Right resizeable panel
      {
        ImGui::SameLine();
        ImGui::BeginChild("SelectedDeamonRight", ImVec2(0, 0), true);

        ImGui::Text("Selected Daemon");
        ImGui::SameLine();

        if (ImGui::Button("Save")) {
          m_userAgent[m_selectedUserAgentDaemon].save();
        }
        ImGui::SameLine();

        if (ImGui::Button("New")) {
          Utils::File f = Utils::File("NewDaemon", c_userAgentPath, ".plist", "");
          m_userAgent.emplace_back(f);
        }
        ImGui::Separator();
        // bottom panel
        {
          std::string copyPath = m_userAgent[m_selectedUserAgentDaemon].getPath();
          std::string copyName = m_userAgent[m_selectedUserAgentDaemon].getFileName();

          if (ImGui::InputText("file path:", &copyPath)) {
            m_userAgent[m_selectedUserAgentDaemon].getFile().setPath(copyPath);
          }
          if (ImGui::InputText("file name:", &copyName)) {
            m_userAgent[m_selectedUserAgentDaemon].getFile().setName(copyName);
          }

          if (m_selectedUserAgentDaemon >= m_userAgent.size()) {
            m_selectedUserAgentDaemon = 0;
          }
          LaunchDaemon &daemon = m_userAgent[m_selectedUserAgentDaemon];
          renderDaemon(daemon);
        }
        ImGui::EndChild();
      }
    }

    void LaunchDaemonsManager::renderDaemon(HummingBirdCore::System::LaunchDaemon &daemon) {
      Utils::PlistUtil::Plist *plist = daemon.getPlist();

      if (plist != nullptr) {
        int index = 0;
        if (ImGui::Button("Add calendar interval")) {
          plist->addCalendarIntervalToRootNode();
        }
        ImGui::Separator();

        for (auto &node: plist->getRootNode().children) {
          renderNode(node.second, index);
          index += 1;
        }
      }
    }

    void LaunchDaemonsManager::renderNode(HummingBirdCore::Utils::PlistUtil::PlistNode &node, int index) {
      std::string id = node.key + " - " + std::to_string(index);

      if (node.type == Utils::PlistUtil::PlistTypeString) {
        std::string val = std::get<std::string>(node.getValue());
        int colorCount = 0;
        if (node.required && !node.value.has_value()) {
          ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
          colorCount++;
        }

        if (ImGui::InputText(id.c_str(), &val)) {
          node.setValue(val);
        }

        if (colorCount > 0) {
          ImGui::PopStyleColor(colorCount);
        }
      } else if (node.type == Utils::PlistUtil::PlistTypeInteger) {
        int colorCount = 0;
        int val = std::get<int>(node.getValue());
        if (node.required && !node.value.has_value()) {
          ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
          colorCount++;
        }
        if (ImGui::InputInt(id.c_str(), &val)) {
          node.setValue(val);
        }

        if (colorCount > 0) {
          ImGui::PopStyleColor(colorCount);
        }
      } else if (node.type == Utils::PlistUtil::PlistTypeBoolean) {
        int colorCount = 0;
        bool val = std::get<bool>(node.getValue());
        if (node.required && !node.value.has_value()) {
          ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
          colorCount++;
        }
        if (ImGui::Checkbox(id.c_str(), &val)) {
          node.setValue(val);
        }

        if (colorCount > 0) {
          ImGui::PopStyleColor(colorCount);
        }
      } else if (node.type == Utils::PlistUtil::PlistTypeArray) {
        int colorCount = 0;
        if (node.required && !node.value.has_value()) {
          ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
          colorCount++;
        }
        if (ImGui::TreeNode(id.c_str())) {
          int index = 0;
          for (auto &child: node.children) {
            renderNode(child.second, index);
            index++;
          }
          ImGui::TreePop();
        }

        if (colorCount > 0) {
          ImGui::PopStyleColor(colorCount);
        }
      } else if (node.type == Utils::PlistUtil::PlistTypeDictionary) {
        int colorCount = 0;
        if (node.required && !node.value.has_value()) {
          ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
          colorCount++;
        }
        if (ImGui::TreeNode(id.c_str())) {
          int index = 0;
          for (auto &child: node.children) {
            renderNode(child.second, index);
            index++;
          }
          ImGui::TreePop();
        }
      } else if (node.type == Utils::PlistUtil::PlistTypeDate) {
        int colorCount = 0;
        Utils::PlistUtil::PlistNode::Date &date = std::get<Utils::PlistUtil::PlistTypeDate>(node.getValue());
        if (node.required && !node.value.has_value()) {
          ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
          colorCount++;
        }
        if (ImGui::TreeNode(id.c_str())) {
          if (ImGui::InputInt("Weekday: ", &date.weekday)) {
            node.setValue(date);
          }
          if (ImGui::InputInt("Month: ", &date.month)) {
            node.setValue(date);
          }
          if (ImGui::InputInt("Day: ", &date.day)) {
            node.setValue(date);
          }
          if (ImGui::InputInt("Hour: ", &date.hour)) {
            node.setValue(date);
          }
          if (ImGui::InputInt("Minute: ", &date.minute)) {
            node.setValue(date);
          }
          ImGui::TreePop();
        }

        if (colorCount > 0) {
          ImGui::PopStyleColor(colorCount);
        }
      }

      if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip(node.getDescription().c_str());
      }
    }

    void LaunchDaemonsManager::fetchAllDaemons() {
      std::vector<HummingBirdCore::Utils::File> fileUserAgent = HummingBirdCore::Utils::FolderUtils::getFilesInFolder(c_userAgentPath.string(), ".plist");
      std::vector<HummingBirdCore::Utils::File> fileGlobalAgent = HummingBirdCore::Utils::FolderUtils::getFilesInFolder(c_globalAgentPath.string(), ".plist");
      std::vector<HummingBirdCore::Utils::File> fileGlobalDaemon = HummingBirdCore::Utils::FolderUtils::getFilesInFolder(c_globalDaemonPath.string(), ".plist");
      std::vector<HummingBirdCore::Utils::File> fileSystemAgent = HummingBirdCore::Utils::FolderUtils::getFilesInFolder(c_SystemAgentPath.string(), ".plist");
      std::vector<HummingBirdCore::Utils::File> fileSystemDaemon = HummingBirdCore::Utils::FolderUtils::getFilesInFolder(c_SystemDaemonPath.string(), ".plist");

      for (const auto &file: fileUserAgent) {
        m_userAgent.emplace_back(file);
      }

      for (const auto &file: fileGlobalAgent) {
        m_globalAgent.emplace_back(file);
      }

      for (const auto &file: fileGlobalDaemon) {
        m_globalDaemon.emplace_back(file);
      }

      for (const auto &file: fileSystemAgent) {
        m_systemAgent.emplace_back(file);
      }

      for (const auto &file: fileSystemDaemon) {
        m_systemDaemon.emplace_back(file);
      }
    }

    bool LaunchDaemonsManager::renderTab(const std::string &name, const std::vector<LaunchDaemon> &daemons) {
      bool userAgentOpen = true;

      if (ImGui::BeginTabItem(name.c_str(), &userAgentOpen, ImGuiTabItemFlags_None)) {
        int index = 0;

        for (auto &daemon: daemons) {
          if (ImGui::Selectable(daemon.getFileName().c_str(), m_selectedUserAgentDaemon == index)) {
            m_selectedUserAgentDaemon = index;
          }
          index++;
        }

        ImGui::EndTabItem();
      }

      return open;
    }
  }// namespace System
}// namespace HummingBirdCore