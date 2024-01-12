//
// Created by Kasper de Bruin on 15/12/2023.
//
#pragma once

#include <iostream>
#include <sstream>
#include <random>
#include <iomanip>
#include <ctime>

namespace HummingBirdCore::Utils {

  inline static std::string generate_uuid() {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<unsigned long long> dis;

    std::stringstream ss;
    ss << std::hex << std::setfill('0');

    for (int i = 0; i < 8; i++) {
      ss << std::setw(2) << (dis(gen) & 0xff);
    }

    ss << "-";

    for (int i = 0; i < 4; i++) {
      ss << std::setw(2) << (dis(gen) & 0xff);
    }

    ss << "-";
    ss << std::setw(4) << (dis(gen) & 0xffff);
    ss << "-";
    ss << std::setw(4) << (dis(gen) & 0xffff);
    ss << "-";

    for (int i = 0; i < 12; i++) {
      ss << std::setw(2) << (dis(gen) & 0xff);
    }

    return ss.str();
  }

}// namespace HummingBirdCore::Utils