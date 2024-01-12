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
        if(Utils::FileUtils::fileExists(file.getFullPath()) || file.name.empty())
        {
          plist->parsePlist(file.getFullPath());
        }else{
          CORE_INFO("File " + file.getFullPath() + " does not exist, creating a new plist file");
          plist->createNewPlist();
          file.name = "NewDaemon";
          file.extension = ".plist";
        }
      }

  public:
      std::string getLabel() const {
        return file.name;
      }

      Utils::PlistUtil::Plist* getPlist() {
        return plist.get();
      }

  public:
      void save(){
        getPlist()->writePlist(file);
      }
    };

    class LaunchDaemonsManager : public UIWindow {
  public:
      explicit LaunchDaemonsManager(const std::string &name) : UIWindow(ImGuiWindowFlags_None, name), m_userAgent() {
        fetchAllDaemons();
      }

      ~LaunchDaemonsManager() {
      }

      void render() override;
      void renderDaemon(LaunchDaemon &daemon);
      void renderNode(Utils::PlistUtil::PlistNode &node, int index);
      void fetchAllDaemons();

  private:
      int m_selectedDaemon = 0;
      std::vector<LaunchDaemon> m_userAgent = {};

      //Service type paths
      const std::filesystem::path c_userAgentPath = "~/Library/LaunchAgents";

      const std::filesystem::path c_globalAgentPath = "/Library/LaunchAgents";
      const std::filesystem::path c_globalDaemonPath = "/Library/LaunchDaemons";

      const std::filesystem::path c_SystemAgentPath = "/System/Library/LaunchAgents";
      const std::filesystem::path c_SystemDaemonPath = "/System/Library/LaunchDaemons";


      //UI
      const ImGuiWindowFlags c_leftWindowFlags = ImGuiWindowFlags_HorizontalScrollbar;
      const ImGuiChildFlags c_leftChildFlags = ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX;
      const ImGuiTreeNodeFlags c_baseTreeNodeFlagsLeft = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
      const ImGuiSelectableFlags c_selectableFlags = ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_SpanAllColumns;

      bool m_wrapText = false;
    };

  }// namespace System
}// namespace HummingBirdCore
