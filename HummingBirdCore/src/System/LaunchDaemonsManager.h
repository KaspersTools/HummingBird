//
// Created by Kasper de Bruin on 08/01/2024.
//

#pragma once

#include <PCH/pch.h>

#include "Utils/Plist/PlistUtil.h"


namespace HummingBirdCore {
  namespace System {
    enum LaunchDaemonStatus {
      Running,
      OK,
      Stopped,
      Unknown
    };

    struct LaunchDaemon {
  private:
      Utils::File file;
      std::unique_ptr<Utils::PlistUtil::Plist> plist;

  public:
      explicit LaunchDaemon(Utils::File file) : file(file),
                                                plist(std::make_unique<Utils::PlistUtil::Plist>()) {
        CORE_TRACE("Loading plist file: " + file.getFullPath());
        if (Utils::FileUtils::fileExists(file.getFullPath()) || file.getName().empty()) {
          plist->parsePlist(file.getFullPath());
        } else {
          CORE_INFO("File " + file.getFullPath() + " does not exist, creating a new plist file");
          plist->createNewPlist();
          file.setName("NewDaemon");
          file.setExtension(".plist");
        }
      }

  public:
      Utils::PlistUtil::Plist *getPlist() {
        return plist.get();
      }

      Utils::File &getFile() {
        return file;
      }

      const std::string getPath() const {
        return file.getPath();
      }

      const std::string getFileName() const {
        return file.getName();
      }

  public:
      void save() {
        getPlist()->writePlist(file);
      }
    };

    class LaunchDaemonsManager : public UIWindow {
  public:
      explicit LaunchDaemonsManager(const std::string &name) : UIWindow(ImGuiWindowFlags_None, name), m_userAgent()
      , m_globalAgent(), m_globalDaemon(), m_systemAgent(), m_systemDaemon(){
        fetchAllDaemons();
      }

      ~LaunchDaemonsManager() {
      }

      void render() override;
      void renderDaemon(LaunchDaemon &daemon);
      void renderNode(Utils::PlistUtil::PlistNode &node, int index);
      void fetchAllDaemons();
      bool renderTab(const std::string &name, const std::vector<LaunchDaemon> &daemons);

  private:
      int m_selectedUserAgentDaemon = 0;
      std::vector<LaunchDaemon> m_userAgent = {};
      std::vector<LaunchDaemon> m_globalAgent = {};
      std::vector<LaunchDaemon> m_systemAgent = {};
      std::vector<LaunchDaemon> m_globalDaemon = {};
      std::vector<LaunchDaemon> m_systemDaemon = {};

      //Service type paths
      const std::filesystem::path c_userAgentPath = "~/Library/LaunchAgents";

      const std::filesystem::path c_globalAgentPath = "/Library/LaunchAgents";
      const std::filesystem::path c_globalDaemonPath = "/Library/LaunchDaemons";

      const std::filesystem::path c_SystemAgentPath = "/System/Library/LaunchAgents";
      const std::filesystem::path c_SystemDaemonPath = "/System/Library/LaunchDaemons";


      //UI
      const ImGuiWindowFlags c_leftWindowFlags = ImGuiWindowFlags_NoScrollbar;
      const ImGuiChildFlags c_leftChildFlags = ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX;

      bool m_wrapText = false;
    };

  }// namespace System
}// namespace HummingBirdCore
