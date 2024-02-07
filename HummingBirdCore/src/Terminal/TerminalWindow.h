//
// Created by Kasper de Bruin on 01/11/2023.
//
#pragma once
#include <PCH/pch.h>

#include <HBUI/UIWindow.h>

#include "../Folder.h"

#include <csignal>
#include <mutex>

#include <thread>
#include <zconf.h>

//TODO: Move to imgui repo in future
//TODO: Move terminal to seperate file
//TODO: Add colors to logs and terminal
namespace HummingBirdCore::Terminal {
  struct Command {
public:
#ifdef __APPLE__
    Command(const std::string &command, const std::string &location, passwd *pws = nullptr)
        : command(command), location(location), pws(pws) {}
#else
    Command(const std::string &command, const std::string &location)
        : command(command), location(location){}
#endif

    ~Command() {
    }

    std::string getCommand() const { return command; }
    std::string getLocation() const { return location; }

    std::string getRanBy() const {
#ifdef __APPLE__
      if (pws != nullptr) {
        return pws->pw_name;
      }
#endif
      return "Ran by not found";
    }

private:
    const std::string location;
    const std::string command;
#ifdef __APPLE__
    //ran by
    const passwd *pws = nullptr;
#endif
  };

  struct TerminalLog {
public:
    TerminalLog(std::string time, std::string log, Command command) : time(std::move(time)), log(std::move(log)), command(std::move(command)) {
    }
    ~TerminalLog() = default;

    std::string getTime() const { return time; }
    std::string getLog() const { return log; }
    Command getCommand() const { return command; }

private:
    std::string time;
    std::string log;
    Command command;
  };

  class TerminalWindow : public UIWindow {
public:
    TerminalWindow(const std::string& name) : TerminalWindow(ImGuiWindowFlags_None, name) {}
    TerminalWindow(ImGuiWindowFlags flags, const std::string& name) : UIWindow(name, ImGuiWindowFlags_None, false) {
      m_currentFolder->setChildDirectories();
#ifdef __APPLE__
      pws = getpwuid(geteuid());
#endif
    }

    ~TerminalWindow();

    void render() override;

private:
    std::vector<std::string> splitCommand(const std::string &command);

    void executeCommand(const std::string &command);
    void executeCommandThreaded(const Command& cmd);

    /**
     * @brief Custom popen function to execute commands
     * @param command The command to execute
     * @param fp The file pointer to write the output to
     * @return The exit code of the command
     */
    int popenHumming(const char *command, FILE **fp);

private:
    void addLog(std::string log, const Command &command) {
      std::lock_guard<std::mutex> lock(m_logMutex);
      m_logs.emplace_back(TerminalLog(getTimestamp(), log, command));
    }

    void errorLog(std::string log) {
      std::lock_guard<std::mutex> lock(m_logMutex);
      m_logs.emplace_back(TerminalLog(getTimestamp(), log, Command("", "")));
      CORE_ERROR(log);
    }

    void killCurrentCommand(){
      pid_t pid = m_currentPid.load();
      if (pid != -1) {
#ifdef __APPLE__
        kill(pid, SIGTERM);// sends a termination signal
        m_currentPid = -1; // reset the current PID
#endif
      }
    }

    //TODO: USE INPUT CLASS
    void handleInput() {
      if (ImGui::IsItemFocused() || ImGui::IsWindowFocused()) {
        //record kb input and add to input string
        bool inputChange = ImGui::GetIO().InputQueueCharacters.size();
        if (inputChange) {
          scrollToBottom();
          int inputChar = ImGui::GetIO().InputQueueCharacters.front();
          if (inputChar != 0) {
            m_input += (char) inputChar;
          }
        }

        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Backspace))) {
          scrollToBottom();
          if (m_input.size() > 0) {
            m_input.pop_back();
          }
        }

        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter))) {
          scrollToBottom();
          executeCommand(m_input);
          m_input = "";
        }

        //todo: implement up for history etc.
      }
    }
    void scrollToBottom() {
      float maxy = ImGui::GetScrollMaxY();
      ImGui::SetScrollY(maxy);
    }
    std::string getTimestamp() {
      std::time_t now = std::time(nullptr);
      std::tm *ltm = std::localtime(&now);
      char timestamp[10];
      std::strftime(timestamp, sizeof(timestamp), "%H:%M:%S", ltm);
      return std::string(timestamp);
    }

private:
    //Terminal
    std::shared_ptr<Folder> m_currentFolder = std::make_shared<Folder>("/Users/k.debruin/", "k.debruin");
    std::vector<std::string> m_commandQueue;
    std::mutex m_logMutex;
    std::vector<TerminalLog> m_logs;
    std::string m_input;
    std::atomic<pid_t> m_currentPid = -1;// using atomic for thread-safety
    std::thread commandThread;

    //User data
    //get the user name
#ifdef __APPLE__
    struct passwd *pws = nullptr;
#endif

  };
}// namespace HummingBirdCore::Terminal
