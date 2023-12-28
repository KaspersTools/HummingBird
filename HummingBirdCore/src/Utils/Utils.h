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
  namespace Utils {
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
  }

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
