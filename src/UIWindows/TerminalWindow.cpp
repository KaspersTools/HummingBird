//
// Created by Kasper de Bruin on 01/11/2023.
//

#include "TerminalWindow.h"
void TerminalWindow::Render() {

    // Output region
    if (ImGui::BeginChild("OutputRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())))
    {
        for (const std::string& log : logs_)
        {
            ImGui::TextUnformatted(log.c_str());
        }
        if (scrollToBottom_)
            ImGui::SetScrollHereY(1.0f);

        scrollToBottom_ = false;
    }
    ImGui::EndChild();

    // Command-line
    bool isReadOnly = currentPid_ != -1;  // This could be set somewhere in your program logic

    ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll |
                                (isReadOnly ? ImGuiInputTextFlags_ReadOnly : 0);

    if (ImGui::InputText("##CommandLine", inputBuf_, sizeof(inputBuf_),
                         flags ))
    {
        ExecuteCommand(inputBuf_);
        strcpy(inputBuf_, "");
        scrollToBottom_ = true;
    }

    if(ImGui::Button("Stop")){
        StopCurrentCommand();
    }

    // Auto-focus on window appearing
    ImGui::SetItemDefaultFocus();
}