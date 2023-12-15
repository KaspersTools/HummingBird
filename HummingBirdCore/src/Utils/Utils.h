//
// Created by Kasper de Bruin on 10/12/2023.
//
#pragma once

#include <fstream>
#include <iostream>
#include <string>

#ifdef __APPLE__
#include <Security/Security.h>
#endif

#include "../Log.h"

namespace HummingBirdCore {
  struct Folder {
public:
    Folder(std::filesystem::path name, std::filesystem::path location) : name(name), location(location) {
      childFolders = std::vector<std::string>();
      //get all child folders
      for (auto &p: std::filesystem::directory_iterator(location))
        if (p.is_directory()) {
          CORE_TRACE("Found folder: {0}", p.path().string());
          childFolders.emplace_back(p.path().filename().string());
        }
    }

    Folder(std::filesystem::path name, std::filesystem::path location, std::vector<std::string> childFolders) : name(name), location(location), childFolders(childFolders) {}

    ~Folder() = default;

    void changeToChildFolder(int i) {
      changeFolder(childFolders[i]);
    }

    void backFolder() {
      if (m_backwardFolderLocation != "") {
        std::string s = location;
        changeFolder(m_backwardFolderLocation);
        m_forwardFolderlocation = s;
      }
    }

    void forwardFolder() {
      if (m_forwardFolderlocation != "") {
        m_backwardFolderLocation = location;
        changeFolder(m_forwardFolderlocation);
      }
    }

    void changeFolder(std::string folderName) {

      m_backwardFolderLocation = location;
      m_forwardFolderlocation = "";

      location = location / folderName;
      name = folderName;
      childFolders = std::vector<std::string>();

      //get all child folders
      for (auto &p: std::filesystem::directory_iterator(location))
        if (p.is_directory()) {
          CORE_TRACE("Found folder: {0}", p.path().string());
          childFolders.emplace_back(p.path().filename().string());
        }
    }


    std::string getName() const { return name.string(); }
    std::string getLocation() const { return location.string(); }

    std::vector<std::string> getChildFolders() const { return childFolders; }

private:
    std::filesystem::path name;
    std::filesystem::path location;

    std::vector<std::string> childFolders;

    std::string m_forwardFolderlocation = "";
    std::string m_backwardFolderLocation = "";
  };

  class
          FileUtils {
public:
    /// <summary>
    /// Check if a file exists
    /// </summary>
    /// <param name="name">The name of the file</param>
    /// <returns>True if the file exists</returns>
    inline static bool fileExists(const std::string &name) {
      std::ifstream f(name.c_str());
      return f.good();
    }

    inline static std::string readFromFile(const std::string &fileLocation) {
      std::string fileContent;
      std::string line;

      std::ifstream myFile(fileLocation);

      if (!myFile.is_open()) {
        std::cerr << "Unable to open file: " << fileLocation << std::endl;
        return fileContent;
      }

      while (std::getline(myFile, line)) {
        fileContent += line + "\n";
      }

      myFile.close();

      return fileContent;
    }

    inline static std::vector<Folder> getFoldersInLocation(const std::string &location) {

      CORE_TRACE("Getting folders in location: {0}", location);

      std::vector<Folder> r;
      for (auto &p: std::filesystem::directory_iterator(location))
        if (p.is_directory()) {
          CORE_TRACE("Found folder: {0}", p.path().string());
          r.emplace_back(p.path().filename().string(), p.path().string());
        }


      return r;
    }
  };// namespace HummingBirdCore
}// namespace HummingBirdCore
