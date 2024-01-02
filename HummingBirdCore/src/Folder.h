//
// Created by Kasper de Bruin on 19/12/2023.
//

#pragma once
#include <PCH/pch.h>

namespace HummingBirdCore {
  class Folder {
public:
    Folder() = default;
    Folder(const std::filesystem::path &path, const std::string &name) : Path(path), Name(name) {}
    ~Folder() = default;

    void setChildDirectories() {
      SubDirectories.clear();
      for (auto &p: std::filesystem::directory_iterator(Path)) {
        try {
          if (std::filesystem::is_directory(p.symlink_status())) {
            if (std::filesystem::is_symlink(p.symlink_status())) {
              std::filesystem::path real_path = std::filesystem::read_symlink(p.path());
              if (real_path == Path) {
                continue;// Avoid adding if it points back to the parent directory
              }
            }
            SubDirectories.emplace_back(p.path(), p.path().filename().string());
          }
        } catch (const std::filesystem::filesystem_error &e) {
          CORE_ERROR("Error: {0}", e.what());
        }
      }
    }


    std::filesystem::path Path;
    std::string Name;
    std::vector<Folder> SubDirectories = {};
  };
}// namespace HummingBirdCore