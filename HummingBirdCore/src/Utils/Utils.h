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


    struct File {

  private:
      std::filesystem::path path;
      std::string name;
      std::string extension;

  public:
      std::string content;
      bool saved = true;

      File() = default;

      File(const std::string &name, const std::filesystem::path &path, const std::string &extension, const std::string &content) : name(name), path(path), extension(extension), content(content) {
        setPath(path.string());
      }

  public:
      const std::string getName() const {
        return name;
      }
      std::string getExtension() const {
        return extension;
      }


      std::string getPath() const {
        return path.string();
      }

      std::string getFullPath() const {
        if (path.string().contains("~")) {
          std::string homeDir = getenv("HOME");
          return homeDir + path.string().substr(1, path.string().length()) + "/" + name + extension;
        }

        return path.string() + "/" + name + extension;
      }
      std::string getFullPathWithExtension() const {
        return path.string() + "/" + name + extension;
      }

      //todo: more filechecking
      void setName(const std::string &newName) {
        name = newName;
      }
      void setExtension(const std::string &newExtension) {
        extension = newExtension;
      }
      void setPath(const std::filesystem::path &newPath) {
        if (path.string().contains("~")) {
          std::string homeDir = getenv("HOME");
          path = homeDir + newPath.string().substr(1, newPath.string().length());
          return;
        }
        path = newPath;
      }

      void setPath(const std::string &newPath) {
        setPath(std::filesystem::path(newPath));
      }
    };

    namespace FileUtils {
      inline static bool fileExists(const std::string &fullname) {
        return std::filesystem::exists(fullname);
      }
      inline static bool fileExists(const File &file) {
        std::string fullpath = file.getFullPath();
        CORE_TRACE("Checking if file exists " + fullpath);
        return fileExists(fullpath);
      }
      inline static bool readFromFile(const std::string &path, std::string *contents) {
        std::ifstream myFile(path);

        if (!myFile.is_open()) {
          CORE_ERROR("Unable to open file: " + path);
          return false;
        }

        std::stringstream buffer;
        buffer << myFile.rdbuf();
        *contents = buffer.str();

        myFile.close();

        return true;
      }
      inline static bool readFromFile(const std::string &path, std::vector<uint8_t> *contents, size_t offset = 0, std::optional<size_t> length = std::nullopt) {
        FILE *fp = fopen(path.c_str(), "rb");

        if (fp == nullptr) {
          return false;
        }

        if (std::fseek(fp, 0, SEEK_END) != 0) {
          std::fclose(fp);
          return false;
        }

        long size = std::ftell(fp);
        if (size == (long) -1) {
          std::fclose(fp);
          return false;
        }

        if (length) {
          if (offset + *length > static_cast<size_t>(size)) {
            std::fclose(fp);
            return false;
          }

          size = *length;
        }

        if (std::fseek(fp, offset, SEEK_SET) != 0) {
          std::fclose(fp);
          return false;
        }

        *contents = std::vector<uint8_t>(size);

        if (size > 0) {
          if (std::fread(contents->data(), size, 1, fp) != 1) {
            std::fclose(fp);
            return false;
          }
        }

        std::fclose(fp);
      }

      inline static File getFile(const std::filesystem::path &fileLocation) {
        std::filesystem::path fullLocation = fileLocation.string();

        if (!fileExists(fullLocation.string())) {
          CORE_ERROR("File {0} does not exist", fullLocation.string());
          return File();
        }

        File file;

        file.setPath(fullLocation.parent_path());
        file.setName(fullLocation.stem().string());
        file.setExtension(fullLocation.extension().string());

        readFromFile(fullLocation.string(), &file.content);

        CORE_TRACE("File " + file.getFullPath() + " loaded");

        return file;
      }

      inline static bool writeToFile(const File &file, const std::string &content) {
        std::ofstream myFile(file.getFullPath());

        if (!myFile.is_open()) {
          CORE_ERROR("Unable to open file: " + file.getFullPath());
          return false;
        }

        myFile << content;

        myFile.close();

        return true;
      }

      inline static bool createFile(const File &file) {
        file.getFullPath();
        std::ofstream myFile(file.getFullPath());

        if (!myFile.is_open()) {
          CORE_ERROR("Unable to open file: " + file.getFullPath());
          return false;
        }

        myFile << file.content;

        myFile.close();

        return true;
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
  }// namespace Utils
}// namespace HummingBirdCore
