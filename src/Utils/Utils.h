//
// Created by Kasper de Bruin on 10/12/2023.
//
#pragma once

#include <fstream>
#include <string>

#include <Security/Security.h>
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
      AuthorizationRef authorizationRef;
      OSStatus status = AuthorizationCreate(NULL, kAuthorizationEmptyEnvironment, kAuthorizationFlagDefaults, &authorizationRef);
      if (status != errAuthorizationSuccess) {
        std::cerr << "Authorization failed: " << status << std::endl;
        return "";
      }

      AuthorizationItem authItem = {kAuthorizationRightExecute, 0, NULL, 0};
      AuthorizationRights authRights = {1, &authItem};
      AuthorizationFlags flags = kAuthorizationFlagDefaults | kAuthorizationFlagInteractionAllowed | kAuthorizationFlagPreAuthorize | kAuthorizationFlagExtendRights;

      status = AuthorizationCopyRights(authorizationRef, &authRights, kAuthorizationEmptyEnvironment, flags, NULL);
      if (status != errAuthorizationSuccess) {
        std::cerr << "AuthorizationCopyRights failed: " << status << std::endl;
        AuthorizationFree(authorizationRef, kAuthorizationFlagDefaults);
        return "";
      }

      // Use the authorized reference to write to the file
      std::ifstream file;
      file.open(fileLocation);
      if (!file.is_open()) {
        std::cerr << "Unable to open file: " << fileLocation << std::endl;
        AuthorizationFree(authorizationRef, kAuthorizationFlagDefaults);
        return "";
      }

      std::string fileContent;
      std::string line;
      while (getline(file, line)) {
        fileContent += line + "\n";
      }

      file.close();
      AuthorizationFree(authorizationRef, kAuthorizationFlagDefaults);
      return fileContent;
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
      AuthorizationRef authorizationRef;
      OSStatus status = AuthorizationCreate(NULL, kAuthorizationEmptyEnvironment, kAuthorizationFlagDefaults, &authorizationRef);
      if (status != errAuthorizationSuccess) {
        std::cerr << "Authorization failed: " << status << std::endl;
        return false;
      }

      AuthorizationItem authItem = {kAuthorizationRightExecute, 0, NULL, 0};
      AuthorizationRights authRights = {1, &authItem};
      AuthorizationFlags flags = kAuthorizationFlagDefaults | kAuthorizationFlagInteractionAllowed | kAuthorizationFlagPreAuthorize | kAuthorizationFlagExtendRights;

      status = AuthorizationCopyRights(authorizationRef, &authRights, kAuthorizationEmptyEnvironment, flags, NULL);
      if (status != errAuthorizationSuccess) {
        std::cerr << "AuthorizationCopyRights failed: " << status << std::endl;
        AuthorizationFree(authorizationRef, kAuthorizationFlagDefaults);
        return false;
      }

      // Use the authorized reference to write to the file
      std::ofstream file;
      file.open(filePath);

      if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filePath << std::endl;
        AuthorizationFree(authorizationRef, kAuthorizationFlagDefaults);
        return false;
      }

      file << content;

      file.close();
      AuthorizationFree(authorizationRef, kAuthorizationFlagDefaults);
      return true;
    }


    inline static bool writeToFileWithAuthorization(const std::string &filePath, const std::vector<std::string> &lines) {
#ifdef __APPLE__
      //Authorize
      AuthorizationRef authorizationRef;
      OSStatus status = AuthorizationCreate(NULL, kAuthorizationEmptyEnvironment, kAuthorizationFlagDefaults, &authorizationRef);
      if (status != errAuthorizationSuccess) {
        std::cerr << "Authorization failed: " << status << std::endl;
        return false;
      }

      AuthorizationItem authItem = {kAuthorizationRightExecute, 0, NULL, 0};
      AuthorizationRights authRights = {1, &authItem};
      AuthorizationFlags flags = kAuthorizationFlagDefaults | kAuthorizationFlagInteractionAllowed | kAuthorizationFlagPreAuthorize | kAuthorizationFlagExtendRights;

      status = AuthorizationCopyRights(authorizationRef, &authRights, kAuthorizationEmptyEnvironment, flags, NULL);
      if (status != errAuthorizationSuccess) {
        std::cerr << "AuthorizationCopyRights failed: " << status << std::endl;
        AuthorizationFree(authorizationRef, kAuthorizationFlagDefaults);
        return false;
      }

      // Use the authorized reference to write to the file
      std::ofstream file;
      file.open(filePath);

      if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filePath << std::endl;
        AuthorizationFree(authorizationRef, kAuthorizationFlagDefaults);
        return false;
      }

#endif
    }

    inline static bool saveFile(const std::string &filePath, const std::string &content, const bool asAdmin = false) {
      std::vector<std::string> lines;
      lines.push_back(content);
      return saveFile(filePath, lines, asAdmin);
    }

    inline static bool saveFile(const std::string &filePath, const std::vector<std::string> &lines, const bool asAdmin = false) {
      if (asAdmin) {
        return writeToFileWithAuthorization(filePath, lines);
      }

      std::ofstream file;
      file.open(filePath, std::ios::out | std::ios::trunc);
      if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filePath << std::endl;
        return false;
      }

      for (const auto &line: lines) {
        file << line << std::endl;
      }

      file.close();
      return true;
    }
  };
}// namespace HummingBirdCore
