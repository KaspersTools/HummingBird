//
// Created by Kasper de Bruin on 01/11/2023.
//

#include "TerminalWindow.h"
#include "../Utils/Input.h"


namespace HummingBirdCore::Terminal {
  //TERMINAL
  TerminalWindow::~TerminalWindow() {
  }

  //PUBLIC
  void TerminalWindow::render() {
    // Set tm background color
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.12f, 0.12f, 0.12f, 1.00f));

    //Terminal area
    ImGui::BeginChild("TerminalScrollArea", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));


    //    ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "Last login: %s", getTimestamp().c_str());
    for (int i = 0; i < m_logs.size(); i++) {
      ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "%s", m_logs[i].getTime().c_str());
      ImGui::SameLine();
      ImGui::TextWrapped("%s", std::string(m_logs[i].getCommand().getRanBy() + " " + m_logs[i].getLog()).c_str());
    }
    ImGui::PopStyleVar();
    ImGui::Separator();

    // Command input
    ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "%s", std::string(getTimestamp() + " " + pws->pw_name).c_str());
    ImGui::SameLine();
    ImGui::TextWrapped("%s", m_input.c_str());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));
    if ((int) (ImGui::GetTime() / 0.4) % 2) {
      ImGui::SameLine();
      ImGui::Text("|");
    }
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();

    //Create invisible button to focus the window
    ImGui::SetCursorPos(ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

    if (ImGui::Button("##focus", ImVec2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y))) {
      CORE_TRACE("Terminal focused");
      ImGui::SetWindowFocus();
    }

    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar(2);
    ImGui::SetItemAllowOverlap();
    handleInput();
    ImGui::EndChild();

    ImGui::End();

    if(Input::isLeftCtrlPressed() && Input::isKeyPressed(SDLK_c)){
      CORE_TRACE("Killing current command");
      killCurrentCommand();
    }

    //    for (int i = 0; i < m_logs.size(); i++) {
    //      ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "%s", m_logs[i].time.c_str());
    //      ImGui::SameLine();
    //      ImGui::TextWrapped("%s", m_logs[i].log.c_str());
    //    }
    //
    //    if (ImGui::InputText("Input", m_inputBuf, IM_ARRAYSIZE(m_inputBuf), ImGuiInputTextFlags_EnterReturnsTrue)) {
    //      addLog(m_inputBuf);
    //      m_inputBuf[0] = '\0';
    //    }

    // Draw the input text box next to the prefix
    //    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), false,
    //                      ImGuiWindowFlags_HorizontalScrollbar);
    //    // Display each item in the command history

    //
    //    // Draw the command line prefix e.g. "user@hostname:~$ "
    //    ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "%s", "user@hostname:~$ ");
    //    ImGui::SameLine();
    //    ImGui::InputText("##command", m_inputBuf, IM_ARRAYSIZE(m_inputBuf), ImGuiInputTextFlags_EnterReturnsTrue);
    //    ImGui::EndChild();
    //    // Add blinking cursor effect by changing the text color


    //    // Output region

    //    ImGui::SameLine();
    //    if (ImGui::Button("Stop")) {
    //      stopCurrentCommand();
    //    }
    //
    //    // Auto-focus on window appearing
    //    ImGui::SetItemDefaultFocus();
  }

  //PRIVATE
  void TerminalWindow::executeCommand(const std::string &command) {
    const std::vector<std::string> commandsToRun = splitCommand(command);
    for (const std::string &command: commandsToRun) {
      //create a command
      const Command& cmd{command, m_currentFolder->Path, pws};
      addLog(command,cmd);

      killCurrentCommand();

      //run the command threaded
      commandThread = std::thread(&TerminalWindow::executeCommandThreaded, this, cmd);
      commandThread.detach();
      //wait till the command is done

    }
  }

  std::vector<std::string> TerminalWindow::splitCommand(const std::string &command) {
    std::vector<std::string> commands;
    std::string currentCommand = "";
    for (int i = 0; i < command.size(); i++) {
      if (command[i] == '&') {
        if (command[i + 1] == '&') {
          commands.push_back(currentCommand);
          currentCommand = "";
          i++;
        } else {
          currentCommand += command[i];
        }
      } else {
        currentCommand += command[i];
      }
    }
    commands.push_back(currentCommand);
    return commands;
  }

  void TerminalWindow::executeCommandThreaded(const Command& command) {
    FILE *fp = popen(command.getCommand().c_str(), "r");
    int pipeFd;

    // Create a pipe and execute the command in a child process
    if ((pipeFd = popenHumming(command.getCommand().c_str(), &fp)) == -1) {
        addLog("Failed to run command: " + command.getCommand(), command);
      return;
    }

    // Store the PID of the process
    m_currentPid = pipeFd;

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), fp) != nullptr) {
      // Remove newline character if present
      size_t len = strlen(buffer);
      if (len > 0 && buffer[len - 1] == '\n') buffer[len - 1] = '\0';
        addLog(buffer, command);
    }

    // Reset the PID to -1 after the command has finished
    m_currentPid = -1;
    pclose(fp);
  }

  int TerminalWindow::popenHumming(const char *command, FILE **fp){
    int pipeFd[2];
    pid_t pid;

    if (pipe(pipeFd) == -1) {
      return -1;
    }

    if ((pid = fork()) == -1) {
      close(pipeFd[0]);
      close(pipeFd[1]);
      return -1;
    }

    if (pid == 0) {// child process
      close(pipeFd[0]);
      if (pipeFd[1] != STDOUT_FILENO) {
        dup2(pipeFd[1], STDOUT_FILENO);
        close(pipeFd[1]);
      }
      if (pipeFd[1] != STDERR_FILENO) {
        dup2(pipeFd[1], STDERR_FILENO);
        close(pipeFd[1]);
      }
      execl("/bin/sh", "sh", "-c", command, nullptr);
      _exit(127);
    }

    *fp = fdopen(pipeFd[0], "r");
    if (*fp == nullptr) {
      close(pipeFd[0]);
      return -1;
    }

    return pid;
  }

}// namespace HummingBirdCore::Terminal
