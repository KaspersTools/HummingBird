//
// Created by Kasper de Bruin on 01/11/2023.
//

#pragma once

#include <PCH/pch.h>

#ifdef __APPLE__
#include <Security/Security.h>
#endif

#include "../../UIWindows/UIWindow.h"

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
      HostsFileLine(TokenType type, std::string ip, std::string hostname, bool enabled) : c_type(type), c_originalIP(ip),
                                                                                          c_originalHostname(hostname),
                                                                                          c_originalEnabled(enabled),
                                                                                          m_ip(ip), m_hostname(hostname),
                                                                                          enabled(enabled) {}

      void setIP(const char string[256]) {
        m_ip = string;
      }

      void setHostname(const char string[256]) {
        m_hostname = string;
      }

      std::string getOriginalIP() const {
        return c_originalIP;
      }

      std::string getOriginalHostname() const {
        return c_originalHostname;
      }

      std::string getHostname() const {
        return m_hostname;
      }
      std::string getIP() const {
        return m_ip;
      }

      bool isCommnent() const {
        return c_type == COMMENT;
      }

      bool isHostname() const {
        return c_type == HOSTNAME;
      }

      bool isIPv4() const {
        return c_type == IPV4_str;
      }

      bool isIPv6() const {
        return c_type == IPV6_str;
      }

      bool isNewline() const {
        return c_type == NEWLINE;
      }

      bool isUnknown() const {
        return c_type == UNKNOWN;
      }

      void reset() {
        m_ip = c_originalIP;
        m_hostname = c_originalHostname;
        enabled = c_originalEnabled;
      }

  public:
      bool enabled;

  private:
      const std::string c_originalIP;
      const std::string c_originalHostname;
      const bool c_originalEnabled;
      const TokenType c_type;

      std::string m_ip;
      std::string m_hostname;
    };

public:
    EditHostsWindow(const std::string &name) : EditHostsWindow(ImGuiWindowFlags_None, name) {}
    EditHostsWindow(const ImGuiWindowFlags flags, const std::string &name) : UIWindow(name, flags) {
      parseHostsFile();
    }

    void render() override;

private:
    [[nodiscard]] const bool isIPv4str(const std::string &str) const {
      return std::regex_match(str, c_ipv4_regex);
    }

    [[nodiscard]] const bool isIPv6str(const std::string &str) const {
      return std::regex_match(str, c_ipv6_regex);
    }

    [[nodiscard]] const bool isComment(const std::string &str) {
      return std::regex_match(str, c_comment_regex);
    }

    [[nodiscard]] const bool isHostname(const std::string &str) const {
      return std::regex_match(str, c_hostname_regex);
    }

    [[nodiscard]] const bool isUnknown(const std::string &str) const {
      return std::regex_match(str, c_hostname_regex);
    }

    [[nodiscard]] const bool isNewline(const std::string &str) const {
      return std::regex_match(str, c_hostname_regex);
    }

    void parseHostsFile() {
      //open /etc/hosts
      std::ifstream hostsFile(c_hostsPath);
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
              std::string toCheck = token.substr(1, token.size() - 1);
              TokenType type = getTokenType(toCheck);

              if (type == IPV4_str) {
                std::string hostname;
                if (ss >> hostname) {
                  HostsFileLine hostsLine = {type, toCheck, hostname, false};
                  m_hostsFileLines.push_back(hostsLine);
                }
              } else if (type == IPV6_str) {
                std::string hostname;
                if (ss >> hostname) {
                  HostsFileLine hostsLine = {type, toCheck, hostname, false};
                  m_hostsFileLines.push_back(hostsLine);
                }
              } else {
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

    TokenType getTokenType(const std::string &str) {
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

private:
    const std::string c_askPassPath = "Assets/scripts/askpass.sh";
#ifdef __linux__
    const std::string c_hostsPath = "/etc/hosts";
    const std::string c_tempFilePath = "/tmp/hosts_temp";
#elif __APPLE__// macOS specific code
    const std::string c_tempFilePath = "/tmp/hosts_temp";
    const std::string c_hostsPath = "/etc/hosts";
#elif _WIN64   // Windows specific code
    const std::string c_hostsPath = "C:\\Windows\\System32\\drivers\\etc\\hosts";
    const std::string c_tempFilePath = "C:\\AppData\\Local\\Temp\\hosts_temp";
#endif
    std::vector<HostsFileLine> m_hostsFileLines;

    const std::regex c_ipv4_regex = std::regex(R"((\b25[0-5]|\b2[0-4][0-9]|\b[01]?[0-9][0-9]?)(\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)){3})");
    const std::regex c_ipv6_regex = std::regex(R"((([0-9a-fA-F]{1,4}:){7,7}[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,7}:|([0-9a-fA-F]{1,4}:){1,6}:[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,5}(:[0-9a-fA-F]{1,4}){1,2}|([0-9a-fA-F]{1,4}:){1,4}(:[0-9a-fA-F]{1,4}){1,3}|([0-9a-fA-F]{1,4}:){1,3}(:[0-9a-fA-F]{1,4}){1,4}|([0-9a-fA-F]{1,4}:){1,2}(:[0-9a-fA-F]{1,4}){1,5}|[0-9a-fA-F]{1,4}:((:[0-9a-fA-F]{1,4}){1,6})|:((:[0-9a-fA-F]{1,4}){1,7}|:)|fe80:(:[0-9a-fA-F]{0,4}){0,4}%[0-9a-zA-Z]{1,}|::(ffff(:0{1,4}){0,1}:){0,1}((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\.){3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])|([0-9a-fA-F]{1,4}:){1,4}:((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\.){3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])))");
    const std::regex c_hostname_regex = std::regex(R"(([A-Za-z0-9]+[^#\n: \t]*))");
    const std::regex c_comment_regex = std::regex(R"((#[^\n]*))");
  };
}// namespace HummingBirdCore::System
