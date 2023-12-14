//
// Created by Kasper de Bruin on 10/12/2023.
//
#pragma once

#include <fstream>
#include <string>
#include <iostream>

#include "../Log.h"

namespace HummingBirdCore {
  class
          FileUtils {
public:
    inline static std::vector<std::string> readLinesFromFile(const std::string &fileLocation) {
      std::vector<std::string> lines;

      std::ifstream file(fileLocation);
      std::string line;

      if (!file.is_open()) {
        std::cerr << "Unable to open file: " << fileLocation << std::endl;
        return lines;
      }

      while (std::getline(file, line)) {
        lines.push_back(line);
      }
      file.close();
      return lines;
    }

    inline static std::string readFromFileWithAuthorization(const std::string &fileLocation) {
      return "";
    }

    inline static std::string readFromFile(const std::string &fileLocation, bool adminRights = false) {
      if (adminRights) {
        return readFromFileWithAuthorization(fileLocation);
      }
      std::string fileContent;
      std::string line;

      std::ifstream myFile(fileLocation);
    }

    inline static std::string readFromFile(const std::string &filelocation) {
      std::string filecontent;
      std::string line;

      std::ifstream myFile(filelocation);

      if (myFile.is_open()) {
        //TODO: EXCEPTION HANDLING
        while (getline(myFile, line)) {
          //remove tabs and replace with spaces (4 spaces)
          for (int i = 0; i < line.length(); i++) {
            if (line[i] == '\t') {
              line.replace(i, 1, "    ");
            }
          }
          filecontent += line + "\n";
        }
        myFile.close();
      } else {
        filecontent = "Unable to open file " + filelocation + "\n";
        CORE_ERROR("Unable to open file {0}", filelocation);
      }
      return filecontent;
    }

    //TODO: Make functions for reading and writing to files with admin rights
    inline static bool writeToFileWithAuthorization(const std::string &filePath, const std::string &content) {

      return true;
    }


    inline static bool writeToFileWithAuthorization(const std::string &filePath, const std::vector<std::string> &lines) {
      return true;
    }

    inline static bool saveFile(const std::string &filePath, const std::string &content, const bool asAdmin = false) {
      return true;
    }

    inline static bool saveFile(const std::string &filePath, const std::vector<std::string> &lines, const bool asAdmin = false){
          return true;
    }
  };// namespace HummingBirdCore
}
