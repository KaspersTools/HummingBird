//
// Created by Kasper de Bruin on 08/01/2024.
//

#pragma once

#include <PCH/pch.h>

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
      std::string name;
      Utils::File file;

      bool enabled;
      LaunchDaemonStatus status;


  public:
      LaunchDaemon() = default;
      LaunchDaemon(const std::string &name, const Utils::File &file, bool enabled, LaunchDaemonStatus status) : name(name), file(file), enabled(enabled), status(status) {
      }

      LaunchDaemon copy() const {
        return LaunchDaemon(name, file, enabled, status);
      }

      bool operator==(const LaunchDaemon &other) const {
        if(this == NULL || &other == NULL){
          return false;
        }
        return name == other.name && file.path == other.file.path;
      }

  public:
      [[nodiscard]] const std::string &getName() const {
        return name;
      }

      void setName(const std::string &name) {
        LaunchDaemon::name = name;
        LaunchDaemon::file.saved = false;
      }

      [[nodiscard]] const Utils::File &getFile() const {
        return file;
      }

      void setFile(const Utils::File &file) {
        LaunchDaemon::file = file;
        LaunchDaemon::file.saved = false;
      }

      [[nodiscard]] bool isEnabled() const {
        return enabled;
      }

      bool& getEnabled() {
        return enabled;
      }

      void setEnabled(bool enabled) {
        LaunchDaemon::enabled = enabled;
        LaunchDaemon::file.saved = false;
      }

      [[nodiscard]] LaunchDaemonStatus getStatus() const {
        return status;
      }

      void setStatus(LaunchDaemonStatus status) {
        LaunchDaemon::status = status;
        LaunchDaemon::file.saved = false;
      }
    };

    class LaunchDaemonsManager : public UIWindow {
  public:
      explicit LaunchDaemonsManager(const std::string &name) : UIWindow(ImGuiWindowFlags_None, name) {
        fetchAllDaemons();
      }

      ~LaunchDaemonsManager(){
        for (auto &daemon : m_userAgent) {
          delete daemon;
        }
        for (auto &daemon : m_globalAgent) {
          delete daemon;
        }
        for (auto &daemon : m_globalDaemons) {
          delete daemon;
        }
        for (auto &daemon : m_systemAgent) {
          delete daemon;
        }
        for (auto &daemon : m_systemDaemons) {
          delete daemon;
        }
      }

      void render() override;

      void fetchAllDaemons();
      void selectDaemon(LaunchDaemon *daemon);
      void renderDaemonsTable(std::vector<LaunchDaemon *> daemons);

  private:
      LaunchDaemon* m_selectedDaemon = nullptr;

      std::vector<LaunchDaemon*> m_userAgent;

      std::vector<LaunchDaemon*> m_globalAgent;
      std::vector<LaunchDaemon*> m_globalDaemons;

      std::vector<LaunchDaemon*> m_systemAgent;
      std::vector<LaunchDaemon*> m_systemDaemons;


      //Service type paths
      const std::filesystem::path c_userAgentPath = "~/Library/LaunchAgents";

      const std::filesystem::path c_globalAgentPath = "/Library/LaunchAgents";
      const std::filesystem::path c_globalDaemonPath = "/Library/LaunchDaemons";

      const std::filesystem::path c_SystemAgentPath = "/System/Library/LaunchAgents";
      const std::filesystem::path c_SystemDaemonPath = "/System/Library/LaunchDaemons";

      const ImGuiWindowFlags     c_leftWindowFlags = ImGuiWindowFlags_HorizontalScrollbar;
      const ImGuiChildFlags      c_leftChildFlags = ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX;
      const ImGuiTreeNodeFlags   c_baseTreeNodeFlagsLeft = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
      const ImGuiSelectableFlags c_selectableFlags = ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_SpanAllColumns;
    };

  }// namespace System
}// namespace HummingBirdCore
