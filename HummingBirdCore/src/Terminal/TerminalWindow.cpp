//
// Created by Kasper de Bruin on 01/11/2023.
//

#include "TerminalWindow.h"

namespace HummingBirdCore::Terminal {

  TerminalWindow::~TerminalWindow() {
    //    s_terminalCount--;
    //    stopCurrentCommand();
  }

  //PUBLIC
  void TerminalWindow::render() {

    if (ImGui::Button("Run stop command")) {
    }

    if (ImGui::Button("Change dir to /opt")) {
      executeCommand("cd /opt");
    }

    if (ImGui::Button("Change dir to ~")) {
      executeCommand("cd ~");
    }

    if (ImGui::Button("List all files in current dir")) {
      executeCommand("ls");
    }

    if (ImGui::Button("Run Ping google.com")) {
      executeCommand("ping google.com");
    }

    if(ImGui::Button("Run Ping google.com && ls")){
      executeCommand("ping google.com && ls");
    }

    if(ImGui::Button("Run Ping google.com && ls && cd /opt")){
      executeCommand("ping google.com && ls && cd /opt");
    }

    if(ImGui::Button("Run cd ~ && ping google.com && ls && cd /opt")){
      executeCommand("cd ~ && ping google.com && ls && cd /opt");
    }

    //    // Output region
    //    if (ImGui::BeginChild("OutputRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()))) {
    //      for (const std::string &log: m_logs) {
    //        ImGui::TextUnformatted(log.c_str());
    //      }
    //      if (m_scrollToBottom)
    //        ImGui::SetScrollHereY(1.0f);
    //
    //      m_scrollToBottom = false;
    //    }
    //    ImGui::EndChild();
    //
    //    // Command-line
    //    bool isReadOnly = m_currentPid != -1;// This could be set somewhere in your program logic
    //
    //    ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll |
    //                                (isReadOnly ? ImGuiInputTextFlags_ReadOnly : 0);
    //
    //    ImGui::Text("%s", std::string(this->currentDir + " >").c_str());
    //    ImGui::SameLine();
    //    if (ImGui::InputText("##CommandLine", m_inputBuf, sizeof(m_inputBuf),
    //                         flags)) {
    //      //add cd currentdir to the command
    //      std::string myString(m_inputBuf);
    //      //        std::string command = std::string(m_inputBuf);
    //      executeCommand(myString);
    //
    //      strcpy(m_inputBuf, "");
    //      m_scrollToBottom = true;
    //    }
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

    //then run the commands
    for(const std::string& command : commandsToRun){
      CORE_TRACE("Running command: {0}", command);
    }
  }

  void TerminalWindow::executeCommandThreaded() {

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
}// namespace HummingBirdCore::Terminal
