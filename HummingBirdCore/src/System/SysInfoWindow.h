//
// Created by Kasper de Bruin on 27/12/2023.
//

#pragma once

#include <PCH/pch.h>

#include "SysInfo.h"
#include "UIWindows/UIWindow.h"


namespace HummingBirdCore::System {
  class SysInfoWindow : public UIWindow {
public:
    SysInfoWindow(const std::string& name) : UIWindow(ImGuiWindowFlags_None, name) {
      m_system.open();
      fetchData();
    }

    ~SysInfoWindow() {}

    void render() override;

private:
    void fetchData() {
      CORE_TRACE("Fetching system data");

      m_memoryUsage = m_system.getMemoryUsage();
      m_cpuTemp = m_system.getCpuTemp();
      m_cpuUsage = m_system.getCpuUsage();
      m_fanCount = m_system.getFanCount();

      for (int i = 0; i < m_fanCount; ++i) {
        m_minSpeeds.push_back(m_system.getMinFanSpeed(i));
        m_maxSpeeds.push_back(m_system.getMaxFanSpeed(i));
        m_currentSpeeds.push_back(m_system.getCurrentFanSpeed(i));
      }

      m_batteryCount = m_system.getBatteryCount();
      m_isCharging = m_system.isChargingBattery();
      m_batteryHealth = m_system.getBatteryHealth();
      m_batteryCycles = m_system.getBatteryCycles();
      m_batteryCharge = m_system.getBatteryCharge();

      m_diskCount = m_system.getDiskCount();
      m_diskNames = m_system.getDiskNames();
      m_diskSpaceTotal = m_system.getDiskSpaceTotal();
      m_diskSpaceUsed = m_system.getDiskSpaceUsed();
      m_diskSpaceFree = m_system.getDiskSpaceFree();

      m_networkData = m_system.getNetworkInterfaces();

      m_logs = {};
      m_logs = m_system.getLogs();
    }

private:
    bool m_tableView = false;

    HummingbirdCore::System::SysInfo m_system = HummingbirdCore::System::SysInfo();

    float m_lastFetchTime = 0.0f;

    std::vector<float> m_memoryUsage;

    int m_cpuTemp;
    std::vector<float> m_cpuUsage;

    int m_fanCount;
    std::vector<int> m_minSpeeds;
    std::vector<int> m_maxSpeeds;
    std::vector<int> m_currentSpeeds;

    int m_batteryCount;
    bool m_isCharging;
    float m_batteryHealth;
    int m_batteryCycles;
    int m_batteryCharge;

    int m_diskCount;
    std::vector<std::string> m_diskNames;
    std::vector<float> m_diskSpaceTotal;
    std::vector<float> m_diskSpaceUsed;
    std::vector<float> m_diskSpaceFree;

    std::vector<HummingbirdCore::System::NetworkData> m_networkData;

    std::string m_logs;
  };
}// namespace HummingBirdCore::System
