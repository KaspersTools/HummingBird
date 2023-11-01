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
namespace KBTools {
    class TerminalWindow : public UIWindow {
    public:
        TerminalWindow() = default;

        void Render() override;

        void ExecuteCommand(const char *command) {
            AddLog(std::string("> ") + command);
            // Start a new thread to run the command
            std::thread(&TerminalWindow::ExecuteCommandThreaded, this, std::string(command)).detach();
        }

    private:
        char m_inputBuf[256] = {0};
        std::vector<std::string> m_logs;
        bool m_scrollToBottom = true;

        std::mutex m_logMutex;
        std::atomic<pid_t> m_currentPid = -1;  // using atomic for thread-safety


        void ExecuteCommandThreaded(const std::string command) {
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
    };
}

#endif //KBTOOLS_TERMINALWINDOW_H
