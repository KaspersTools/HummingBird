//
// Created by Kasper de Bruin on 01/11/2023.
//

#pragma once

#include "../../UIWindows/UIWindow.h"
#include "../../Utils/Utils.h"

#include <iostream>
#include <regex>

namespace HummingBird::System {
  class EditHostsWindow : public KBTools::UIWindow {
public:
    EditHostsWindow() : EditHostsWindow(ImGuiWindowFlags_None) {}
    explicit EditHostsWindow(const ImGuiWindowFlags flags) : UIWindow(flags) {
      m_file_content = FileUtils::readFromFile(m_hostsPath);
    }

    void Render() override;

private:
    const bool isIPv6Address(const std::string &address) const {
      return std::regex_match(address, m_ipv6AddressRegex);
    }

    const bool isIPv4Address(const std::string &address) const {
      return std::regex_match(address, m_ipv4AddressRegex);
    }

    const bool isComment(const std::string &address) const {
      return std::regex_match(address, m_commentRegex);
    }

    const bool isHostname(const std::string &address) const {
      return std::regex_match(address, m_hostnameRegex);
    }

private:
    const std::string m_hostsPath = "Assets/tests/edithosts.txt";

    std::string m_file_content;

    const std::regex m_ipv6AddressRegex = std::regex(R"(^([0-9a-fA-F]{1,4}:){7}[0-9a-fA-F]{1,4}$)");
    const std::regex m_ipv4AddressRegex = std::regex(R"(^([0-9]{1,3}\.){3}[0-9]{1,3}$)");
    const std::regex m_commentRegex = std::regex(R"(#[^\n]*)");
    const std::regex m_hostnameRegex = std::regex(R"([A-Za-z0-9]+[^\n#: \t]*)");
  };

}// namespace HummingBird::System
