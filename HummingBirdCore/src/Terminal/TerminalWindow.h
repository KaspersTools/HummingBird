//
// Created by Kasper de Bruin on 01/11/2023.
//
#pragma once

#include "../UIWindows/UIWindow.h"

//#include <vector>
#include "../Utils/Utils.h"
#include <csignal>
//#include <filesystem>
#include <iostream>
#include <mutex>
#include <thread>
#include <zconf.h>


namespace HummingBirdCore::Terminal {
  struct Command {
public:
    Command(std::string name, std::string command, std::string location)
        : name(name), command(command), location(location) {}

    ~Command() {
    }

    std::string getName() const { return name; }
    std::string getCommand() const { return command; }
    std::string getLocation() const { return location; }

    void executeCommand() {
    }

private:
    const std::string name;
    const std::string location;
    const std::string command;
  };

  class TerminalWindow : public UIWindow {
public:
    TerminalWindow() : TerminalWindow(ImGuiWindowFlags_None) {}
    TerminalWindow(ImGuiWindowFlags flags) : UIWindow(ImGuiWindowFlags_None, "Terminal") {}

    ~TerminalWindow();

    void render() override;

    void executeCommand(const std::string &command);
private:
    //split the command by && and then execute them one by one
    std::vector<std::string> splitCommand(const std::string &command);

    //run command in a thread
    void executeCommandThreaded();

    void addLog(const std::string &log){
      std::lock_guard<std::mutex> lock(m_logMutex);
      m_logs.emplace_back(log);
    }
private:
    //So we always start in the homefolder ? TODO: maybe change this to start in last folder when TerminalWindow is closed??
    //Folder m_currentFolder = Folder("~");

    std::vector<std::string> m_commandQueue;

    std::mutex               m_logMutex;
    std::vector<std::string> m_logs;

    //    inline static uint       s_terminalCount = 0;
    //    std::string              currentDir = "~";
    //    char                     m_inputBuf[256] = {0};
    //    std::vector<std::string> m_logs;
    //    bool                     m_scrollToBottom = true;
    //
    //    std::mutex         m_logMutex;
    //    std::atomic<pid_t> m_currentPid = -1;// using atomic for thread-safety
    //
    //    void executeCommand(std::string &command) {
    //      addLog(this->currentDir + "> " + command);
    //      // Trim whitespace from both ends of the command
    //      command.erase(0, command.find_first_not_of(" \t\n\r\f\v"));
    //      command.erase(command.find_last_not_of(" \t\n\r\f\v") + 1);
    //
    //      if (command.empty()) return;
    //
    //      if (command.starts_with("cd ")) {
    //        std::string newDir = command.substr(3);
    //
    //        if (newDir.starts_with("~")) {
    //          char *home = std::getenv("HOME");// Get the home directory path
    //          if (home == nullptr) {
    //            std::cout << "Error: Home directory not found." << std::endl;
    //            return;
    //          }
    //          newDir = std::string(home) + newDir.substr(1);// Replace ~ with home directory path
    //        }
    //
    //        // Trim whitespace from both ends of the newDir
    //        newDir.erase(0, newDir.find_first_not_of(" \t\n\r\f\v"));
    //        newDir.erase(newDir.find_last_not_of(" \t\n\r\f\v") + 1);
    //
    //        // Check if the newDir is not empty
    //        if (newDir.empty()) {
    //          std::cout << "Error: No directory specified." << std::endl;
    //          return;
    //        }
    //
    //        // Handle relative and absolute paths
    //        std::cout << currentDir << std::endl;
    //        std::filesystem::path newPath = (std::filesystem::path(currentDir) / newDir).lexically_normal();
    //
    //        // Check if the directory exists and is accessible
    //        if (std::filesystem::exists(newPath) && std::filesystem::is_directory(newPath)) {
    //          this->currentDir = newPath.string();
    //          std::cout << "Directory changed to: " << currentDir << std::endl;
    //        } else {
    //          std::cout << "Error: Directory does not exist or cannot be accessed." << std::endl;
    //        }
    //        return;
    //      }
    //
    //      command = "cd " + this->currentDir + " && " + command;
    //      // Start a new thread to run the command
    //      std::thread(&TerminalWindow::executeCommandThreaded, this, std::string(command)).detach();
    //    }
    //
    //    void executeCommandThreaded(const std::string &command) {
    //      FILE *fp = popen(command.c_str(), "r");
    //      int pipeFd;
    //
    //      // Create a pipe and execute the command in a child process
    //      if ((pipeFd = popenHum(command.c_str(), &fp)) == -1) {
    //        addLog("Failed to run command");
    //        return;
    //      }
    //
    //      // Store the PID of the process
    //      m_currentPid = pipeFd;
    //
    //      char buffer[128];
    //      while (fgets(buffer, sizeof(buffer), fp) != nullptr) {
    //        // Remove newline character if present
    //        size_t len = strlen(buffer);
    //        if (len > 0 && buffer[len - 1] == '\n') buffer[len - 1] = '\0';
    //
    //        addLog(buffer);
    //      }
    //
    //      // Reset the PID to -1 after the command has finished
    //      m_currentPid = -1;
    //      pclose(fp);
    //    }
    //
    //    void addLog(const std::string &log) {
    //      std::lock_guard<std::mutex> lock(m_logMutex);
    //      m_logs.emplace_back(log);
    //    }
    //
    //    void stopCurrentCommand() {
    //      pid_t pid = m_currentPid.load();
    //      if (pid != -1) {
    //        kill(pid, SIGTERM);// sends a termination signal
    //        m_currentPid = -1; // reset the current PID
    //      }
    //    }
    //
    //    int popenHum(const char *command, FILE **fp) {
    //      int pipeFd[2];
    //      pid_t pid;
    //
    //      if (pipe(pipeFd) == -1) {
    //        return -1;
    //      }
    //
    //      if ((pid = fork()) == -1) {
    //        close(pipeFd[0]);
    //        close(pipeFd[1]);
    //        return -1;
    //      }
    //
    //      if (pid == 0) {// child process
    //        close(pipeFd[0]);
    //        dup2(pipeFd[1], STDOUT_FILENO);
    //        dup2(pipeFd[1], STDERR_FILENO);
    //        close(pipeFd[1]);
    //
    //        execl("/bin/sh", "sh", "-c", command, NULL);
    //        exit(1);// if execl fails
    //      } else {  // parent process
    //        close(pipeFd[1]);
    //        *fp = fdopen(pipeFd[0], "r");
    //        return pid;
    //      }
    //    }
  };
}// namespace HummingBirdCore::Terminal
