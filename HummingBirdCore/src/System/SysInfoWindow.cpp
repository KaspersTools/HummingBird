//
// Created by Kasper de Bruin on 27/12/2023.
//

#include "SysInfoWindow.h"
#include "Application.h"
#include <chrono>

void HummingBirdCore::System::SysInfoWindow::render() {
  //Check if we need to update
  static auto lastFrameTime = std::chrono::high_resolution_clock::now();
  auto now = std::chrono::high_resolution_clock::now();
  float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(now - lastFrameTime).count();
  lastFrameTime = now;

  m_lastFetchTime += deltaTime;

  if (m_lastFetchTime >= 1.0f) {
    fetchData();
    m_lastFetchTime = 0.0f;
  }

  if (ImGui::BeginTabBar("SysInfoTabBar")) {
    if (ImGui::BeginTabItem("Memory")) {

      std::string freeMem = std::to_string(m_memoryUsage[0]);
      std::string activeMem = std::to_string(m_memoryUsage[1]);
      std::string inactiveMem = std::to_string(m_memoryUsage[2]);
      std::string wiredMem = std::to_string(m_memoryUsage[3]);
      std::string compressedMem = std::to_string(m_memoryUsage[4]);

      ImGui::Text("Memory Usage");

      ImGui::Text("Free: %s GB", freeMem.c_str());
      ImGui::ProgressBar(m_memoryUsage[0] / m_system.getTotalMemory(), ImVec2(-1.0f, 0.0f), "Free");
      ImGui::Text("Active: %s GB", activeMem.c_str());
      ImGui::ProgressBar(m_memoryUsage[1] / m_system.getTotalMemory(), ImVec2(-1.0f, 0.0f), "Active");
      ImGui::Text("Inactive: %s GB", inactiveMem.c_str());
      ImGui::ProgressBar(m_memoryUsage[2] / m_system.getTotalMemory(), ImVec2(-1.0f, 0.0f), "Inactive");
      ImGui::Text("Wired: %s GB", wiredMem.c_str());
      ImGui::ProgressBar(m_memoryUsage[3] / m_system.getTotalMemory(), ImVec2(-1.0f, 0.0f), "Wired");
      ImGui::Text("Compressed: %s GB", compressedMem.c_str());
      ImGui::ProgressBar(m_memoryUsage[4] / m_system.getTotalMemory(), ImVec2(-1.0f, 0.0f), "Compressed");

      ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("CPU")) {

      ImGui::Text("CPU Temperature: %d°C", m_cpuTemp);

      ImGui::Text("CPU Usage");
      ImGui::Text("User: %.2f%%", m_cpuUsage[0]);
      ImGui::ProgressBar(m_cpuUsage[0] / 100, ImVec2(-1.0f, 0.0f), "User");
      ImGui::Text("System: %.2f%%", m_cpuUsage[1]);
      ImGui::ProgressBar(m_cpuUsage[1] / 100, ImVec2(-1.0f, 0.0f), "System");
      ImGui::Text("Idle: %.2f%%", m_cpuUsage[2]);
      ImGui::ProgressBar(m_cpuUsage[2] / 100, ImVec2(-1.0f, 0.0f), "Idle");
      ImGui::Text("Nice: %.2f%%", m_cpuUsage[3]);
      ImGui::ProgressBar(m_cpuUsage[3] / 100, ImVec2(-1.0f, 0.0f), "Nice");

      ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("Fans")) {

      ImGui::Text("Fan Count: %d", m_fanCount);

      for (int i = 0; i < m_fanCount; ++i) {
        ImGui::Text("Fan %d", i);
        ImGui::Text("Min Speed: %d RPM", m_minSpeeds[i]);
        ImGui::Text("Max Speed: %d RPM", m_maxSpeeds[i]);
        ImGui::Text("Current Speed: %d RPM", m_currentSpeeds[i]);
      }

      ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("Battery")) {

      ImGui::Text("Battery Count: %d", m_batteryCount);
      ImGui::Text("Battery Count: %d", m_batteryCount);

      ImGui::Text("Charging: %s", m_isCharging ? "Yes" : "No");

      ImGui::Text("Battery Health: %.2f%%", m_batteryHealth);

      ImGui::Text("Battery Cycles: %d", m_batteryCycles);
      ImGui::Text("Battery Charge: %d%%", m_batteryCharge);

      ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("Disks")) {
      ImGui::Text("Disks");

      for (int i = 0; i < m_diskCount; ++i) {
        ImGui::Text("Disk %d", i);
        ImGui::Text("Name: %s", m_diskNames[i].c_str());
        std::string label = "Free: " + std::to_string(m_diskSpaceFree[i]) + " GB of the " +
                            std::to_string(m_diskSpaceTotal[i]) + " GB";
        ImGui::ProgressBar(m_diskSpaceUsed[i] / m_diskSpaceTotal[i], ImVec2(-1.0f, 0.0f), label.c_str());
      }
      ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("Network")) {
      ImGui::Text("Network");
//      if (ImGui::BeginTable("Network Data Table", 10)) {// 10 Columns for each data point
//        ImGui::TableSetupColumn("Name");
//        ImGui::TableSetupColumn("IP");
//        ImGui::TableSetupColumn("MAC");
//        ImGui::TableSetupColumn("Netmask");
//        ImGui::TableSetupColumn("Broadcast");
//        ImGui::TableSetupColumn("Gateway");
//        ImGui::TableSetupColumn("MTU");
//        ImGui::TableSetupColumn("Interface Type");
//        ImGui::TableSetupColumn("Status");
//        ImGui::TableSetupColumn("Speed");
//        ImGui::TableHeadersRow();
//
//        for (const auto &data: m_networkData) {
//          ImGui::TableNextRow();
//          ImGui::TableSetColumnIndex(0);
////            ImGui::Text("%s", data.name.c_str());
////            ImGui::TableSetColumnIndex(1);
////            ImGui::Text("%s", data.ip.c_str());
////            ImGui::TableSetColumnIndex(2);
////            ImGui::Text("%s", data.mac.c_str());
////            ImGui::TableSetColumnIndex(3);
////            ImGui::Text("%s", data.netmask.c_str());
////            ImGui::TableSetColumnIndex(4);
////            ImGui::Text("%s", data.broadcast.c_str());
////            ImGui::TableSetColumnIndex(5);
////            ImGui::Text("%s", data.gateway.c_str());
////            ImGui::TableSetColumnIndex(6);
////            ImGui::Text("%d", data.mtu);
////            ImGui::TableSetColumnIndex(7);
////            ImGui::Text("%s", data.interfaceType.c_str());
////            ImGui::TableSetColumnIndex(8);
////            ImGui::Text("%s", data.status.c_str());
////            ImGui::TableSetColumnIndex(9);
////            ImGui::Text("%d", data.speed);
//        }
//      }


      if (m_networkData.empty()) {
        ImGui::Text("No network data available.");
      } else {
        for (const auto &data: m_networkData) {
          if (ImGui::TreeNode(data.name.c_str())) {
            ImGui::Text("IPV4 Address: %s", data.ipv4.c_str());
            ImGui::Text("Netmask: ipv4 %s", data.netmaskipv4.c_str());
            ImGui::Text("Broadcast Address: ipv4 %s", data.broadcastipv4.c_str());
            ImGui::Text("IPV6 Address: %s", data.ipv6.c_str());
            ImGui::Text("Netmask: ipv6 %s", data.netmaskipv6.c_str());
            ImGui::Text("Broadcast Address: ipv6 %s", data.broadcastipv6.c_str());
            ImGui::Text("MAC Address: %s",  data.mac.c_str());
            ImGui::Text("Gateway: %s", data.gateway.c_str());
            ImGui::Text("Status: %s", data.status.c_str());
            ImGui::Text("Interface Type: %s", data.interfaceType.c_str());
            ImGui::Text("MTU: %u", data.mtu);
            ImGui::Text("Interface Index: %d", data.interfaceIndex);
            ImGui::Text("Speed: %lu", data.speed);
            ImGui::Text("Using DHCP: %s", data.usingDhcp ? "Yes" : "No");

            if (!data.dnsServers.empty()) {
              ImGui::Text("DNS Servers:");
              for (const auto &server: data.dnsServers) {
                ImGui::BulletText("%s", server.c_str());
              }
            }

            ImGui::TreePop();
          }
        }
      }

      ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("Bluetooth")) {
      ImGui::Text("Bluetooth");
      ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("Processes")) {
      ImGui::Text("Processes");
      ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("Audio")) {
      ImGui::Text("Audio");
      ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("USB")) {
      ImGui::Text("USB");
      ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("Logs")) {
      ImGui::Text("Logs");
      ImGui::TextWrapped("%s", m_logs.c_str());
      ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("About")) {
      ImGui::Text("About");
      ImGui::EndTabItem();
    }

    ImGui::EndTabBar();
  }
}
