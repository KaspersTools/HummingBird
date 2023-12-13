//
// Created by Kasper de Bruin on 01/11/2023.
//

#pragma once

#include "../../UIWindows/UIWindow.h"
#include "../../Utils/Utils.h"

#include <iostream>
#include <regex>
#include <sstream>

namespace HummingBirdCore::System {
  class EditHostsWindow : public HummingBirdCore::UIWindow {
private:
    enum TokenType {
      IPV4_str,
      IPV6_str,
      COMMENT,
      HOSTNAME,
      NEWLINE,
      UNKNOWN
    };

    struct HostsFileLine {
  public:
        HostsFileLine(TokenType type, std::string ip, std::string hostname, bool enabled) :
                                                                            type(type),
                                                                            originalIP(ip), originalHostname(hostname),
                                                                            ip(ip), hostname(hostname),
                                                                            originalEnabled(enabled), enabled(enabled)
      {}

        void setIP(const char string[256]) {
          ip = string;
        }

        void setHostname(const char string[256]) {
          hostname = string;
        }

        const std::string getOriginalIP() const {
            return originalIP;
        }

        const std::string getOriginalHostname() const {
            return originalHostname;
        }

        const std::string getHostname() const {
            return hostname;
        }
        const std::string getIP() const {
            return ip;
        }

        const bool isCommnent() const {
            return type == COMMENT;
        }

        const bool isHostname() const {
            return type == HOSTNAME;
        }

        const bool isIPv4() const {
            return type == IPV4_str;
        }

        const bool isIPv6() const {
            return type == IPV6_str;
        }

        const bool isNewline() const {
            return type == NEWLINE;
        }

        const bool isUnknown() const {
            return type == UNKNOWN;
        }

        void reset() {
          ip = originalIP;
          hostname = originalHostname;
          enabled = originalEnabled;
        }

    public:
      bool enabled;
    private:
        TokenType type;
        const std::string originalIP;
        const std::string originalHostname;
        const bool originalEnabled;

        std::string ip;
        std::string hostname;

    };

public:
    EditHostsWindow() : EditHostsWindow(ImGuiWindowFlags_None) {}
    explicit EditHostsWindow(const ImGuiWindowFlags flags) : UIWindow(flags) {
      parseHostsFile();
    }

    void Render() override;

private:
    const bool isIPv4str(const std::string &str) const {
      return std::regex_match(str, ipv4_regex);
    }

    const bool isIPv6str(const std::string &str) const {
      return std::regex_match(str, ipv6_regex);
    }

    const bool isComment(const std::string &str) const {
      return std::regex_match(str, comment_regex);
    }

    const bool isHostname(const std::string &str) const {
      return std::regex_match(str, hostname_regex);
    }

    const bool isUnknown(const std::string &str) const {
      return std::regex_match(str, hostname_regex);
    }

    const bool isNewline(const std::string &str) const {
      return std::regex_match(str, hostname_regex);
    }

private:
#ifdef __linux__ // Linux specific code
    const std::string m_hostsPath = "/etc/hosts";
#elif __APPLE__ // macOS specific code
    const std::string m_hostsPath = "/etc/hosts";
#elif _WIN64 // Windows specific code
    const std::string m_hostsPath = "C:\\Windows\\System32\\drivers\\etc\\hosts";
#endif


    std::vector<HostsFileLine> m_hostsFileLines;

    const std::regex ipv4_regex     = std::regex(R"((\b25[0-5]|\b2[0-4][0-9]|\b[01]?[0-9][0-9]?)(\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)){3})");
    const std::regex ipv6_regex     = std::regex(R"((([0-9a-fA-F]{1,4}:){7,7}[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,7}:|([0-9a-fA-F]{1,4}:){1,6}:[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,5}(:[0-9a-fA-F]{1,4}){1,2}|([0-9a-fA-F]{1,4}:){1,4}(:[0-9a-fA-F]{1,4}){1,3}|([0-9a-fA-F]{1,4}:){1,3}(:[0-9a-fA-F]{1,4}){1,4}|([0-9a-fA-F]{1,4}:){1,2}(:[0-9a-fA-F]{1,4}){1,5}|[0-9a-fA-F]{1,4}:((:[0-9a-fA-F]{1,4}){1,6})|:((:[0-9a-fA-F]{1,4}){1,7}|:)|fe80:(:[0-9a-fA-F]{0,4}){0,4}%[0-9a-zA-Z]{1,}|::(ffff(:0{1,4}){0,1}:){0,1}((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\.){3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])|([0-9a-fA-F]{1,4}:){1,4}:((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\.){3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])))");
    const std::regex hostname_regex = std::regex(R"(([A-Za-z0-9]+[^#\n: \t]*))");
    const std::regex comment_regex  = std::regex(R"((#[^\n]*))");


    void parseHostsFile() {
      //open /etc/hosts
      std::ifstream hostsFile(m_hostsPath);
      std::string line;

      while (std::getline(hostsFile, line)) {
        // Use a stringstream to split the line into tokens
        std::stringstream ss(line);
        std::string token;
        while (ss >> token) {
          TokenType type = getTokenType(token);
          switch (type) {
            case IPV4_str: {
              // Expecting a hostname after the IP address
              std::string hostname;
              if (ss >> hostname) {
                  HostsFileLine hostsLine = {type, token, hostname, true};
                  m_hostsFileLines.push_back(hostsLine);
              }
              break;
            }
            case IPV6_str: {
              // Expecting a hostname after the IP address
              std::string hostname;
              if (ss >> hostname) {
                HostsFileLine hostsLine = {type, token, hostname, true};
                m_hostsFileLines.push_back(hostsLine);
              }
              break;
            }
            case COMMENT: {
              //Do another check to see if it's a ipv4 or ipv6 address from the comment\
                // Expecting a hostname after the IP address
                std::string to_check = token.substr(1, token.size() - 1);
                TokenType type = getTokenType(to_check);

                if(type == IPV4_str) {
                  std::string hostname;
                  if (ss >> hostname) {
                    HostsFileLine hostsLine = {type, to_check, hostname, false};
                    m_hostsFileLines.push_back(hostsLine);
                  }
                } else if(type == IPV6_str) {
                  std::string hostname;
                  if (ss >> hostname) {
                    HostsFileLine hostsLine = {type, to_check, hostname, false};
                    m_hostsFileLines.push_back(hostsLine);
                  }
                }else {
                  HostsFileLine hostsLine = {TokenType::COMMENT, token, "", true};
                  m_hostsFileLines.push_back(hostsLine);
                }

              break;
            }
            case HOSTNAME:
              // Handle standalone hostnames, if applicable
              break;
            case UNKNOWN:
              // Handle unknown tokens
              break;
            default:
              break;
          }
        }
      }

      hostsFile.close();
    }

    TokenType getTokenType(const std::string& str) {
      if (isIPv6str(str)) {
        return IPV6_str;
      } else if (isIPv4str(str)) {
        return IPV4_str;
      } else if (isComment(str)) {
        return COMMENT;
      } else if (isHostname(str)) {
        return HOSTNAME;
      } else if (str == "\n") {
        return NEWLINE;
      } else {
        return UNKNOWN;
      }
    }
  };
}// namespace HummingBirdCore::System
