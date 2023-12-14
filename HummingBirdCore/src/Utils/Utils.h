//
// Created by Kasper de Bruin on 10/12/2023.
//
#pragma once

#include <fstream>
#include <string>
#include <iostream>

#ifdef __APPLE__
#include <Security/Security.h>
#endif

#include "../Log.h"

namespace HummingBirdCore {
  class
          FileUtils {
public:
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
  };// namespace HummingBirdCore
}
