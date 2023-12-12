//
// Created by Kasper de Bruin on 01/11/2023.
//

#include "EditHostsWindow.h"

void HummingBird::System::EditHostsWindow::Render() {
  //  ImGui::Tex

  //Make imgui table
  if (ImGui::BeginTable("Hosts", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable)) {
    ImGui::TableSetupColumn("Use");
    ImGui::TableSetupColumn("IP");
    ImGui::TableSetupColumn("Hostname");

    //For loop
    bool use = false;

    for (int row = 0; row < 10; ++row)
    {
      ImGui::TableNextRow(); // Move to the next row

      for (int column = 0; column < 3; ++column)
      {
        ImGui::TableSetColumnIndex(column);
        if (column == 0)
        {
          // First column - add a checkbox
          bool checked = true; // You can set this based on your data
          ImGui::Checkbox("##use", &checked);
        }
        else if (column == 1)
        {
          // Second column - add the IP address (fake data)
          ImGui::Text("127.0.0.1");
        }
        else if (column == 2)
        {
          // Third column - add the hostname (fake data)
          ImGui::Text("localhost%d.lan", row); // Append the row number to make it unique
        }
      }
    }
    ImGui::EndTable();
  }
}

