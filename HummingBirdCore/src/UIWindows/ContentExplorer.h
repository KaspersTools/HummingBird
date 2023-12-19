//
// Created by Kasper de Bruin on 17/12/2023.
//

#pragma once

#include "../Rendering/Texture.h"
#include "../Utils/Utils.h"
#include "Rendering/Texture.h"
#include "UIWindow.h"


namespace HummingBirdCore {

  class Folder {
public:
    Folder() = default;
    Folder(const std::filesystem::path &path, const std::string &name) : Path(path), Name(name) {}
    ~Folder() = default;

    std::filesystem::path Path;
    std::string Name;
    std::vector<Folder> SubDirectories = {};
  };

  class ContentBrowserItem {
public:
    ContentBrowserItem(const std::filesystem::path &path, const std::string &name, const Texture *icon)
        : c_path(path), c_name(name), c_icon(icon) {}

    ~ContentBrowserItem() {
      if (c_icon != nullptr)
        delete c_icon;
    }

    const std::filesystem::path &getPath() const { return c_path; }
    const std::string &getName() const { return c_name; }
    const Texture *getIcon() const { return c_icon; }

private:
    const std::filesystem::path c_path;
    const std::string c_name;
    const Texture *c_icon = nullptr;
  };

  class ContentFolderItem : public ContentBrowserItem {
public:
    ContentFolderItem(const std::filesystem::path &path, const std::string &name) : ContentBrowserItem(path, name, nullptr),
                                                                                    m_folder(path, name) {
      setChildDirectories();
    }

    ~ContentFolderItem(){};

    const Folder &getFolder() const { return m_folder; }

    void setChildDirectories() {
      m_folder.SubDirectories.clear();
      for (auto &p: std::filesystem::directory_iterator(m_folder.Path))
        if (p.is_directory()) {
          m_folder.SubDirectories.emplace_back(p.path(), p.path().filename().string());
        }
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
      ContentExplorer() : ContentExplorer(ImGuiWindowFlags_None) {}
      ContentExplorer(ImGuiWindowFlags flags) : UIWindow(ImGuiWindowFlags_None, "Content Explorer") {
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
      //      std::vector<std::shared_ptr<ContentFolderItem>> m_backStack = {};
      //      std::vector<std::shared_ptr<ContentFolderItem>> m_forwardStack = {};
    };

  }// namespace UIWindows
}// namespace HummingBirdCore
