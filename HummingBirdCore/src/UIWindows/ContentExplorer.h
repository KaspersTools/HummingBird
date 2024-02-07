//
// Created by Kasper de Bruin on 17/12/2023.
//

#pragma once

#include <PCH/pch.h>

#include <HBUI/UIWindow.h>

namespace HummingBirdCore {
  class ContentBrowserItem {
public:
    ContentBrowserItem(const std::filesystem::path &path, const std::string &name)
        : c_path(path), c_name(name) {}

    ~ContentBrowserItem() {
    }

    const std::filesystem::path &getPath() const { return c_path; }
    const std::string &getName() const { return c_name; }

private:
    const std::filesystem::path c_path;
    const std::string c_name;

  };

  class ContentFolderItem : public ContentBrowserItem {
public:
    ContentFolderItem(const std::filesystem::path &path, const std::string &name) : ContentBrowserItem(path, name),
                                                                                    m_folder(path, name) {
      m_folder.setChildDirectories();
    }

    ~ContentFolderItem(){};

    const Folder &getFolder() const { return m_folder; }

    void setChildDirectories() {
      m_folder.setChildDirectories();
    }

    std::vector<ContentFolderItem> getSubDirectories() const {
      std::vector<ContentFolderItem> r;
      for (auto &folder: m_folder.SubDirectories) {
        r.emplace_back(folder.Path, folder.Name);
      }
      return r;
    }

private:
    Folder m_folder;
  };

  namespace UIWindows {

    class ContentExplorer : public HummingBirdCore::UIWindow {

  public:
      ContentExplorer(const std::string& name) : ContentExplorer(ImGuiWindowFlags_None, name) {}
      ContentExplorer(ImGuiWindowFlags flags, const std::string& name) : UIWindow(name, ImGuiWindowFlags_None) {
        changeDirectory(*m_currentDirectory);
      }

      ~ContentExplorer() {
      }

      void changeDirectory(const ContentFolderItem &folder) {
        m_backstack.push_back(m_currentDirectory->getPath());
        m_currentDirectory = std::make_shared<ContentFolderItem>(folder.getPath(), folder.getName());
        m_currentDirectory->setChildDirectories();
      }

      void render() override;

  private:
      std::shared_ptr<ContentFolderItem> m_currentDirectory = std::make_shared<ContentFolderItem>("/opt/", "opt");
      std::vector<std::filesystem::path> m_backstack = {};
      std::vector<std::filesystem::path> m_forwardstack = {};
    };

  }// namespace UIWindows
}// namespace HummingBirdCore
