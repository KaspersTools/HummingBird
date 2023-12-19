//
// Created by Kasper de Bruin on 19/12/2023.
//

#pragma once
#include <filesystem>
#include <vector>

namespace HummingBirdCore {
  class Folder {
public:
    Folder() = default;
    Folder(const std::filesystem::path &path, const std::string &name) : Path(path), Name(name) {}
    ~Folder() = default;

    void setChildDirectories() {
      SubDirectories.clear();
      for (auto &p: std::filesystem::directory_iterator(Path))
        if (p.is_directory()) {
          SubDirectories.emplace_back(p.path(), p.path().filename().string());
        }
    }


    std::filesystem::path Path;
    std::string Name;
    std::vector<Folder> SubDirectories = {};
  };
}// namespace HummingBirdCore