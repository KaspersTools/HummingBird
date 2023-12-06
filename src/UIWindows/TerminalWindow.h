//
// Created by Kasper de Bruin on 01/11/2023.
//

#ifndef KBTOOLS_TERMINALWINDOW_H
#define KBTOOLS_TERMINALWINDOW_H


#include "UIWindow.h"
#include <vector>
#include <string>

#include <thread>
#include <mutex>
#include <zconf.h>
#include <csignal>
#include <iostream>
#include <filesystem>

namespace KBTools {
    class TerminalWindow : public UIWindow {
    public:
        TerminalWindow();
        ~TerminalWindow();

        void Render() override;

        void ExecuteCommand(std::string& command) {
            AddLog(this->currentDir +  "> " + command);
            // Trim whitespace from both ends of the command
            command.erase(0, command.find_first_not_of(" \t\n\r\f\v"));
            command.erase(command.find_last_not_of(" \t\n\r\f\v") + 1);

            if(command.empty()) return;

            if(command.starts_with("cd ")) {
                std::string newDir = command.substr(3);

                if(newDir.starts_with("~")) {
                    char* home = std::getenv("HOME"); // Get the home directory path
                    if (home == nullptr) {
                        std::cout << "Error: Home directory not found." << std::endl;
                        return;
                    }
                    newDir = std::string(home) + newDir.substr(1); // Replace ~ with home directory path
                }

                // Trim whitespace from both ends of the newDir
                newDir.erase(0, newDir.find_first_not_of(" \t\n\r\f\v"));
                newDir.erase(newDir.find_last_not_of(" \t\n\r\f\v") + 1);

                // Check if the newDir is not empty
                if(newDir.empty()) {
                    std::cout << "Error: No directory specified." << std::endl;
                    return;
                }

                // Handle relative and absolute paths
                std::cout << currentDir << std::endl;
                std::filesystem::path newPath = (std::filesystem::path(currentDir) / newDir).lexically_normal();

                // Check if the directory exists and is accessible
                if(std::filesystem::exists(newPath) && std::filesystem::is_directory(newPath)) {
                    this->currentDir = newPath.string();
                    std::cout << "Directory changed to: " << currentDir << std::endl;
                } else {
                    std::cout << "Error: Directory does not exist or cannot be accessed." << std::endl;
                }
                return;
            }


            command = "cd " + this->currentDir + " && " + command;
            // Start a new thread to run the command
            std::thread(&TerminalWindow::ExecuteCommandThreaded, this, std::string(command)).detach();
        }

        static std::string GetTerminalName() { return "Terminal " + std::to_string(s_terminalCount); }


    private:
        inline static uint s_terminalCount = 0;
        char m_inputBuf[256] = {0};
        std::vector<std::string> m_logs;
        bool m_scrollToBottom = true;

        std::mutex m_logMutex;
        std::atomic<pid_t> m_currentPid = -1;  // using atomic for thread-safety


        void ExecuteCommandThreaded(const std::string& command) {
            FILE *fp = popen(command.c_str(), "r");
            int pipe_fd;

            // Create a pipe and execute the command in a child process
            if ((pipe_fd = popen2(command.c_str(), &fp)) == -1) {
                AddLog("Failed to run command");
                return;
            }

            // Store the PID of the process
            m_currentPid = pipe_fd;

            char buffer[128];
            while (fgets(buffer, sizeof(buffer), fp) != nullptr) {
                // Remove newline character if present
                size_t len = strlen(buffer);
                if (len > 0 && buffer[len - 1] == '\n') buffer[len - 1] = '\0';

                AddLog(buffer);
            }

            // Reset the PID to -1 after the command has finished
            m_currentPid = -1;
            pclose(fp);
        }

        void AddLog(const std::string &log) {
            std::lock_guard<std::mutex> lock(m_logMutex);
            m_logs.emplace_back(log);
        }

        // New function to stop the currently running command
        void StopCurrentCommand() {
            pid_t pid = m_currentPid.load();
            if (pid != -1) {
                kill(pid, SIGTERM); // sends a termination signal
                m_currentPid = -1;  // reset the current PID
            }
        }

        int popen2(const char *command, FILE **fp) {
            int pipe_fd[2];
            pid_t pid;

            if (pipe(pipe_fd) == -1) {
                return -1;
            }

            if ((pid = fork()) == -1) {
                close(pipe_fd[0]);
                close(pipe_fd[1]);
                return -1;
            }

            if (pid == 0) { // child process
                close(pipe_fd[0]);
                dup2(pipe_fd[1], STDOUT_FILENO);
                dup2(pipe_fd[1], STDERR_FILENO);
                close(pipe_fd[1]);

                execl("/bin/sh", "sh", "-c", command, NULL);
                exit(1); // if execl fails
            } else { // parent process
                close(pipe_fd[1]);
                *fp = fdopen(pipe_fd[0], "r");
                return pid;
            }
        }

        std::string currentDir = "~";
    };

//    uint TerminalWindow::s_terminalCount = 0;
}

#endif //KBTOOLS_TERMINALWINDOW_H
