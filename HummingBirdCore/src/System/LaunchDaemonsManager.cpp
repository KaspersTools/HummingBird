//
// Created by Kasper de Bruin on 08/01/2024.
//

#include "LaunchDaemonsManager.h"

namespace HummingBirdCore {
  namespace System {
    void LaunchDaemonsManager::render() {
      ImGuiWindowFlags finalFlags = m_flags | ImGuiWindowFlags_MenuBar;
      if (!m_selectedDaemon.isSaved()) {
        finalFlags |= ImGuiWindowFlags_UnsavedDocument;
      }

      ImGui::Begin(m_name.c_str(), &m_isOpen, finalFlags);
      //Left resizeable panel
      {
        ImGui::BeginChild("LaunchDaemons", ImVec2(ImGui::GetContentRegionAvail().x * 0.2f, 0), c_leftChildFlags, c_leftWindowFlags);
        ImGui::Text("LaunchDaemons");
        ImGui::Separator();

        static ImVector<int> active_tabs;
        static int next_tab_id = 0;
        if (next_tab_id == 0)// Initialize with some default tabs
          for (int i = 0; i < 5; i++)
            active_tabs.push_back(next_tab_id++);


        // Expose some other flags which are useful to showcase how they interact with Leading/Trailing tabs
        static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyScroll | ImGuiTabBarFlags_TabListPopupButton;

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


        ImGui::SameLine();

        if (ImGui::Button("New")) {
        }
        ImGui::Separator();
        // bottom panel
        {
          std::string path = m_selectedDaemon.getFile().getPath();

          if (ImGui::InputText("file path:", &path)) {
            m_selectedDaemon.getFile().setPath(path);
          }
          LaunchDaemon &daemon = m_selectedDaemon;

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

        //compare nodes
        bool saved = true;
        for (auto &node: plist->getRootNode().children) {
          std::string identifier = node.first;
          Utils::PlistUtil::PlistNode originalNode = m_copyOfSelectedDaemonStart.getPlist()->getRootNode().children[identifier];
          if (originalNode.value.has_value() && !node.second.value.has_value()) {
            saved = false;
          }
          if (!originalNode.value.has_value() && node.second.value.has_value()) {
            saved = false;
          }
          if (originalNode.value.has_value() && node.second.value.has_value()) {
            std::variant<std::string, int, float, bool, Utils::PlistUtil::PlistNode::Date> foundValue = originalNode.getValue();
            std::variant<std::string, int, float, bool, Utils::PlistUtil::PlistNode::Date> currentValue = node.second.getValue();

            if (Utils::PlistUtil::PlistType::PlistTypeString == originalNode.type) {
              std::string originalValueStr = std::get<std::string>(foundValue);
              std::string currentValueStr = std::get<std::string>(currentValue);
              if (originalValueStr != currentValueStr) {
                saved = false;
              }
            }
          }
        }

        for (auto &node: plist->getRootNode().children) {
          renderNode(node.second, index);
          index += 1;
        }
        daemon.setSaved(saved);
      }
    }

    void LaunchDaemonsManager::renderNode(HummingBirdCore::Utils::PlistUtil::PlistNode &node, int index) {
      std::string id = node.key + " - " + std::to_string(index);

      //compare the node m_selectedDaemon with the node m_copyOfSelectedDaemonStart


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
        m_userAgent.emplace_back(std::move(file));
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

    bool LaunchDaemonsManager::selectDaemon(int idx, std::vector<LaunchDaemon> &daemons) {
      if (idx >= daemons.size()) {
        return false;
      }

      if (!daemons[m_selectedIdx].getPlist()->getIsParsed()) {
        CORE_WARN("Plist is not parsed");
        if (!daemons[m_selectedIdx].init()) {
          CORE_ERROR("Failed to init daemon");
          return false;
        }
      }

      m_selectedIdx = idx;
      m_selectedDaemon = daemons[m_selectedIdx];
      m_copyOfSelectedDaemonStart = LaunchDaemon(m_selectedDaemon.getFile());

      return true;
    }

    bool LaunchDaemonsManager::renderTab(const std::string &name, std::vector<LaunchDaemon> &daemons) {
      bool tabOpen = true;

      if (ImGui::BeginTabItem(name.c_str(), &tabOpen, ImGuiTabItemFlags_None)) {
        int index = 0;
        //If new tab is selected select first index if there are items
        if (m_selectedTab != name) {
          CORE_TRACE("selected new tab: " + name);
          m_selectedTab = name;
          selectDaemon(0, daemons);
        }

        for (auto &daemon: daemons) {
          if (ImGui::Selectable(daemon.getFileName().c_str(), m_selectedIdx == index)) {
            selectDaemon(index, daemons);
          }
          index++;
        }

        ImGui::EndTabItem();
      }
      return tabOpen;
    }
  }// namespace System
}// namespace HummingBirdCore