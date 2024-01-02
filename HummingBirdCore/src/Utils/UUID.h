//
// Created by Kasper de Bruin on 15/12/2023.
//
#pragma once

#include <iostream>

namespace HummingBirdCore::Utils {
  class UUID {
public:
    UUID() = default;
    ~UUID() = default;

    static std::string generateUUID() {
      std::string uuid = "";
      uuid += generateUUIDPart();
      uuid += generateUUIDPart();
      uuid += "-";
      uuid += generateUUIDPart();
      uuid += "-";
      uuid += generateUUIDPart();
      uuid += "-";
      uuid += generateUUIDPart();
      uuid += "-";
      uuid += generateUUIDPart();
      uuid += generateUUIDPart();
      uuid += generateUUIDPart();
      return uuid;
    }

private:
    static std::string generateUUIDPart() {
      std::string uuidPart = "";
      for (int i = 0; i < 4; i++) {
        uuidPart += generateRandomChar();
      }
      return uuidPart;
    }

    static char generateRandomChar() {
      int random = rand() % 16;
      if (random < 10) {
        return (char) (random + 48);
      } else {
        return (char) (random + 87);
      }
    }
  };
}// namespace HummingBirdCore::Utils