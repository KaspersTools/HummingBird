//
// Created by Kasper de Bruin on 08/01/2024.
//

#pragma once

#include <PCH/pch.h>z

namespace HummingBirdCore {
  struct LaunchDaemonValue {
    LaunchDaemonValue() = default;

    const std::string key;
    const std::string type;

    std::string value;

    LaunchDaemonValue(const std::string &key, const std::string &value, const std::string &type) : key(key), value(value), type(type) {}
    void setValue(const std::string &value) {
      LaunchDaemonValue::value = value;
    }
  };

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
      LaunchDaemonStatus status;

      plist_t plistData;//type = void*

  public:
      std::map<std::string, LaunchDaemonValue> m_values;

  public:
      LaunchDaemon() = default;
      LaunchDaemon(const Utils::File &file, LaunchDaemonStatus status) : file(file), status(status) {
        init();
      }

      void init() {
        readPlist();

        fetchLabel();
        fetchProgram();
        if (file.name == "com.kasper") {
          fetchStartInterval();
        }
      }

      LaunchDaemon copy() const {
        //TODO: Check what needs to be copied.
        LaunchDaemon daemon;
        return daemon;
      }

  public:
      bool readPlist() {
        if (file.path.empty() || file.path.string().empty()) {
          CORE_ERROR("File path is empty");
          return false;
        }
        if(file.content.empty() || file.content == "")
        {
          CORE_ERROR("File content is empty of file: " + file.path.string());
          return false;
        }
        Utils::PlistUtils::readPlist(file, plistData);
      }

  public:
      const std::map<std::string, LaunchDaemonValue> &getValues() const {
        return m_values;
      }

      Utils::File getFile() const {
        return file;
      }

  private:
      //Default fetch functions
      void fetchLabel() {

        std::string label;

      }

      void fetchProgram() {

        std::string program;

      }

      void fetchStartInterval() {
        int startInterval;
      }

      void fetchProperty(){
        if(file.content.empty())
        {
          CORE_ERROR("File content is empty of file: " + file.path.string());
        }
      }
    };

    class LaunchDaemonsManager : public UIWindow {
  public:
      explicit LaunchDaemonsManager(const std::string &name) : UIWindow(ImGuiWindowFlags_None, name) {
        fetchAllDaemons();
      }

      ~LaunchDaemonsManager() {
        for (auto &daemon: m_userAgent) {
          delete daemon;
        }
        for (auto &daemon: m_globalAgent) {
          delete daemon;
        }
        for (auto &daemon: m_globalDaemons) {
          delete daemon;
        }
        for (auto &daemon: m_systemAgent) {
          delete daemon;
        }
        for (auto &daemon: m_systemDaemons) {
          delete daemon;
        }
      }

      void render() override;

      void fetchAllDaemons();
      void selectDaemon(LaunchDaemon *daemon);
      void renderDaemonsTable(std::vector<LaunchDaemon *> daemons);

  private:
      LaunchDaemon *m_selectedDaemon = nullptr;

      std::vector<LaunchDaemon *> m_userAgent;

      std::vector<LaunchDaemon *> m_globalAgent;
      std::vector<LaunchDaemon *> m_globalDaemons;

      std::vector<LaunchDaemon *> m_systemAgent;
      std::vector<LaunchDaemon *> m_systemDaemons;


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
