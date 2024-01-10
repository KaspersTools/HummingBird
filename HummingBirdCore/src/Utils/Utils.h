//
// Created by Kasper de Bruin on 10/12/2023.
//
#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#ifdef __APPLE__
#include <Security/Security.h>
#endif

#include "../Log.h"

namespace fs = std::filesystem;

namespace HummingBirdCore {
  namespace Utils {
    struct File {
      std::string name;
      std::filesystem::path path;
      std::string extension;
      std::string content;
      bool saved = true;

      File() = default;

      File(const std::string &name, const std::filesystem::path &path, const std::string &extension, const std::string &content) : name(name), path(path), extension(extension), content(content) {
      }
    };

    /**
     * TODO: Copyright stuff here
     */

    /**
     * Takes a string and returns the given string as a FourCharCode.
     * @param givenString   The given string
     * @return The four char code
     */
    static FourCharCode stringToFourCharCode(const std::string &givenString) {
      const char *bytes = givenString.c_str();
      UInt32 byte0 = (unsigned) bytes[0] << (unsigned) 24;
      UInt32 byte1 = (unsigned) bytes[1] << (unsigned) 16;
      UInt32 byte2 = (unsigned) bytes[2] << (unsigned) 8;
      UInt32 byte3 = (unsigned) bytes[3];

      return byte0 | byte1 | byte2 | byte3;
    }

    /**
     * Takes a FourCharCode and returns the given code as a string.
     * @param givenCode The given FourCharCode
     * @return  The four char code
     */
    static std::string fourCharCodeToString(FourCharCode givenCode) {
      char byte0 = givenCode >> (unsigned) 24;
      char byte1 = givenCode >> (unsigned) 16;
      char byte2 = givenCode >> (unsigned) 8;
      char byte3 = givenCode;

      return std::string({byte0, byte1, byte2, byte3});
    }

    /**
     * Converts a given value from the fpe2 data type to int.
     * @param value     The given value as fpe2 type.
     * @return  The corresponding integer value.
     */
    static int fpe2ToInt(UInt8 value[2]) {
      return (value[0] << (unsigned) 6) + (value[1] >> (unsigned) 2);
    }

    /**
     * Converts a given value from the flt data type to int.
     * @param value     The given value as flt type.
     * @return  The corresponding integer value.
     */
    static int fltToInt(UInt8 value[4]) {
      float resultValue = 0;
      std::memcpy(&resultValue, value, 4);
      return (int) resultValue;
    }

    namespace FileUtils {
      inline static bool fileExists(const std::string &fullname) {
        return std::filesystem::exists(fullname);
      }

      inline static std::string readFromFile(const std::string &fileLocation) {
        std::string fileContent;
        std::string line;

        std::ifstream myFile(fileLocation);

        if (!myFile.is_open()) {
          CORE_ERROR("Unable to open file: " + fileLocation);
          return fileContent;
        }

        while (std::getline(myFile, line)) {
          fileContent += line + "\n";
        }

        myFile.close();

        return fileContent;
      }

      inline static File getFile(const std::filesystem::path &fileLocation) {
        std::filesystem::path fullLocation = fileLocation.string();

        if (!fileExists(fullLocation.string())) {
          CORE_ERROR("File {0} does not exist", fullLocation.string());
          return File();
        }

        File file;

        file.path = fullLocation;
        //name = without extension
        file.name = fullLocation.stem().string();
        file.extension = fullLocation.extension().string();
        file.content = readFromFile(fullLocation.string());
        CORE_TRACE("File " + file.name + " loaded");

        return file;
      }
    }// namespace FileUtils

    //#TODO: Remove classes and make them static functions in the right namespace
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

      inline static bool createFolder(const std::string &fullFolderName) {
        if (doesFolderExist(fullFolderName)) {
          CORE_WARN("Folder {0} already exists", fullFolderName);
          return false;
        }

        std::filesystem::create_directories(fullFolderName);
        CORE_TRACE("Folder {0} created", fullFolderName);
        return true;
      }

      inline static std::filesystem::path getHomeDirectory() {
        return getenv("HOME");
      }

      inline static std::vector<HummingBirdCore::Utils::File> getFilesInFolder(const std::string &path) {
        return getFilesInFolder(path, "");
      }

      inline static std::vector<HummingBirdCore::Utils::File> getFilesInFolder(const std::string &path, const std::string &extension) {
        std::vector<HummingBirdCore::Utils::File> files;
        std::filesystem::path fullPath = path;

        if (path.contains("~")) {
          fullPath = getHomeDirectory().string() + path.substr(1, path.length());
        }

        if (!doesFolderExist(fullPath.string())) {
          CORE_ERROR("Folder " + fullPath.string() + " does not exist with the input path: " + path);
          return files;
        }

        for (const auto &entry: std::filesystem::directory_iterator(fullPath)) {
          if (!extension.empty() && entry.path().extension() != extension) {
            continue;
          }

          File file = FileUtils::getFile(entry.path());
          files.push_back(file);
        }
        return files;
      }
    };

    namespace PlistUtils {

    }// namespace PlistUtils
  }// namespace Utils
}// namespace HummingBirdCore
