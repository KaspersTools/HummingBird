//
// Created by Kasper de Bruin on 10/12/2023.
//

#include "EditHosts.h"
#include "../../Log.h"
#include "../../Utils/Utils.h"

#include <fstream>

#define MAX_LINE_LENGTH 256

HummingBird::System::EditHosts::EditHosts(const ImGuiWindowFlags flags, const std::string &filelocation) :
                                         c_filelocation(filelocation), KBTools::UIWindow(flags){
    CORE_TRACE("EditHosts::EditHosts");
    //open the file
    m_filecontent = FileUtils::readFromFile(filelocation);

}

HummingBird::System::EditHosts::~EditHosts() {
    CORE_TRACE("EditHosts::~EditHosts");
}

void HummingBird::System::EditHosts::Render(){
    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Save")) {
            //saveDialog();
            std::ofstream myfile(c_filelocation);
            if (myfile.is_open()) {
                myfile << m_filecontent;
                myfile.close();
            }
            else {
                CORE_ERROR("Unable to open file");
            }
        }
        ImGui::EndMenu();
    }
    ImGui::Text("Edit Hosts");
    ImGui::Separator();
    ImGui::Text("File: %s", c_filelocation.c_str());
    ImGui::Separator();
    char buffer[4096];
    std::strcpy(buffer, m_filecontent.c_str());
    ImGui::InputTextMultiline("##inputText", buffer, sizeof(buffer), ImGui::GetWindowSize() /*text box size*/);
    m_filecontent = buffer;
}

