//
// Created by Kasper de Bruin on 15/12/2023.
//
#pragma once

#include <iostream>
#include "Utils/UUID.h"

namespace HummingBirdCore {

  class ApplicationObject {
public:

    ApplicationObject() {
    }

    ~ApplicationObject() {

    }

    const std::string getUUID() const { return c_uuid; }

private:
    const std::string c_uuid = HummingBirdCore::Utils::UUID::generateUUID();
  };
}