//
// Created by Kasper de Bruin on 10/12/2023.
//
#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>

#ifdef __APPLE__
#include <Security/Security.h>
#endif

#include "../Log.h"

namespace fs = std::filesystem;

namespace HummingBirdCore {
  class FolderUtils {
public:
    inline static bool doesFolderExist(const std::string &fullFolderName) {
      //edge cases
      if (fullFolderName == "~") {
        //get current users home directory
        std::string homeDir = getenv("HOME");
        return std::filesystem::exists(homeDir);
      }
      return std::filesystem::exists(fullFolderName);
    }

//    inline static std::vector<Folder> getFoldersInLocation(const std::string &location) {
//      CORE_TRACE("Getting folders in location: {0}", location);
//
//      std::vector<Folder> r;
//      for (auto &p: std::filesystem::directory_iterator(location))
//        if (p.is_directory()) {
//          CORE_TRACE("Found folder: {0} as an child dir of: {1}", p.path().string(), location);
//          r.emplace_back(p.path().string());
//        }
//      return r;
//    }
  };

  class FileUtils {
public:
    inline static bool fileExists(const std::string &fullname) {
      //TODO:HANDLE EDGE CASES FOR EXAMPLE ~/testfile.txt

      std::fstream f(fullname);
      if (f.good()) {
        CORE_TRACE("File {0} exists", fullname);
        return true;
      }

      if (fullname.find("~") != std::string::npos) {
        std::string homedir = getenv("HOME");
        f = std::fstream(homedir + fullname.substr(1));
      }

      if (!f.good()) {
        CORE_WARN("File {0} does not exist", fullname);
      }
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
  };
}// namespace HummingBirdCore
