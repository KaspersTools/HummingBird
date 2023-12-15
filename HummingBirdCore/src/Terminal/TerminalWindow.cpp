//
// Created by Kasper de Bruin on 01/11/2023.
//

#include "TerminalWindow.h"

namespace HummingBirdCore::Terminal {

  TerminalWindow::~TerminalWindow() {
//    s_terminalCount--;
//    stopCurrentCommand();
  }

  void TerminalWindow::render() {

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
}// namespace HummingBirdCore::Terminal
