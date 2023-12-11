//
// Created by Kasper de Bruin on 10/12/2023.
//

#ifndef KBTOOLS_UTILS_H
#define KBTOOLS_UTILS_H

#endif//KBTOOLS_UTILS_H

#include <string>
#include <fstream>

#include <iostream>

#include "../Log.h"

class FileUtils{
public:
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
    }
    else {
      filecontent = "Unable to open file";
      CORE_ERROR("Unable to open file");
    }
    return filecontent;
  }

};
