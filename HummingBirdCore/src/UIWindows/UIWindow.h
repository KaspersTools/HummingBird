//
// Created by Kasper de Bruin on 01/11/2023.
//

#pragma once

#include "ApplicationObject.h"

#include <chrono>
#include <imgui.h>
#include <imgui_internal.h>
#include <iostream>
#include <map>

// Define a UI window structure
namespace HummingBirdCore {

  struct CurrentFrameStats {
    std::chrono::high_resolution_clock::time_point frameStartTime;
  };
  struct UIWindowRenderStats {
    static const int HISTORY_SIZE = 100;// Adjust as needed
    float frameRenderTimes[HISTORY_SIZE] = {0};
    float averageFrameTimes[HISTORY_SIZE] = {0};
    float peakFrameTimes[HISTORY_SIZE] = {0};
    int currentFrameIndex = 0;// Index for the current frame in the history arrays

    std::string name;// Name of the window

    float frameRenderTime; // Time taken to render the frame
    long frameCount;       // Total number of frames rendered
    float averageFrameTime;// Average time per frame
    float peakFrameTime;   // Peak frame render time
    float updateRate;      // Update rate of the window

    bool showRenderStats = false;

    UIWindowRenderStats(const std::string &name) : name(name), frameRenderTime(0),
                                                   frameCount(0), averageFrameTime(0), peakFrameTime(0), updateRate(0) {}
  };

  class UIWindow : public ApplicationObject {
public:
    UIWindow(ImGuiWindowFlags flags, const std::string &name, const bool autoEndFrame = true) : m_flags(flags), c_name(name), c_autoEndFrame(autoEndFrame) {}

    void beginFrame() {
      startUpdateStats();
      if(!m_isOpen) {
        return;
      }

      ImGui::Begin(c_name.c_str(), &m_isOpen, m_flags | ImGuiWindowFlags_MenuBar);
      if(ImGui::BeginMenuBar()) {
        if(ImGui::BeginMenu("Debug")) {
          if(ImGui::MenuItem("Render Stats")) {
            m_renderStats.showRenderStats = true;
          }
          ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
      }
      render();
      endFrame();
    }

    void endFrame() {
      if (getAutoEndFrame()) {
        ImGui::End();
      }
      endUpdateStats();
      showRenderStatsWindow();
    }

    std::string getName() const { return c_name; }
    bool isOpen() const { return m_isOpen; }

    ImGuiWindowFlags getFlags() const { return m_flags; }

    bool getAutoEndFrame() const { return c_autoEndFrame; }

protected:
    virtual void render() = 0;

private:
    void startUpdateStats() {
      m_currentFrameStats.frameStartTime = std::chrono::high_resolution_clock::now();
    }

    void endUpdateStats() {
      auto end = std::chrono::high_resolution_clock::now();
      std::chrono::duration<float, std::milli> renderDuration = end - m_currentFrameStats.frameStartTime;

      m_renderStats.frameRenderTime = renderDuration.count();
      m_renderStats.frameCount++;
      m_renderStats.averageFrameTime = m_renderStats.frameRenderTime / m_renderStats.frameCount;

      if (m_renderStats.frameRenderTime > m_renderStats.peakFrameTime) {
        m_renderStats.peakFrameTime = m_renderStats.frameRenderTime;
      }

      m_renderStats.updateRate = 1000.0f / m_renderStats.averageFrameTime;

      m_renderStats.frameRenderTimes[m_renderStats.currentFrameIndex % m_renderStats.HISTORY_SIZE] = m_renderStats.frameRenderTime;
      m_renderStats.averageFrameTimes[m_renderStats.currentFrameIndex % m_renderStats.HISTORY_SIZE] = m_renderStats.averageFrameTime;
      m_renderStats.peakFrameTimes[m_renderStats.currentFrameIndex % m_renderStats.HISTORY_SIZE] = m_renderStats.peakFrameTime;

      m_renderStats.currentFrameIndex++;
    }

    void showRenderStatsWindow() {
      if (m_renderStats.showRenderStats) {
        ImGui::Begin(m_renderStats.name.c_str(), &m_renderStats.showRenderStats);
        ImGui::Text("Frame render time: %.3f ms", m_renderStats.frameRenderTime);
        ImGui::Text("Frame count: %ld", m_renderStats.frameCount);
        ImGui::Text("Average frame time: %.3f ms", m_renderStats.averageFrameTime);
        ImGui::Text("Peak frame time: %.3f ms", m_renderStats.peakFrameTime);
        ImGui::Text("Update rate: %.3f fps", m_renderStats.updateRate);

        ImGui::Separator();
        if (ImGui::CollapsingHeader("Plots")) {

          ImGui::Text("Frame render time: %.3f ms", m_renderStats.frameRenderTime);

          //                (const char* label, const float* values,            int values_count,                 int values_offset, const char* overlay_text, float scale_min, float scale_max,                                                                                                      ImVec2 graph_size);
          ImGui::PlotLines("Frame Render Time", m_renderStats.frameRenderTimes, UIWindowRenderStats::HISTORY_SIZE, 0, "Frame Render Time (ms)",                    0.0f,          *std::max_element(m_renderStats.frameRenderTimes, m_renderStats.frameRenderTimes + UIWindowRenderStats::HISTORY_SIZE), ImVec2(0, 80));
          ImGui::PlotLines("Average Frame Time", m_renderStats.averageFrameTimes, UIWindowRenderStats::HISTORY_SIZE, 0, "Average Frame Time (ms)",                 0.0f,          *std::max_element(m_renderStats.averageFrameTimes, m_renderStats.averageFrameTimes + UIWindowRenderStats::HISTORY_SIZE), ImVec2(0, 80));
          ImGui::PlotLines("Peak Frame Time", m_renderStats.peakFrameTimes, UIWindowRenderStats::HISTORY_SIZE, 0, "Peak Frame Time (ms)",                          0.0f,          *std::max_element(m_renderStats.peakFrameTimes, m_renderStats.peakFrameTimes + UIWindowRenderStats::HISTORY_SIZE), ImVec2(0, 80));
        }
        ImGui::End();
      }
    }


public:
    bool m_isOpen = true;
    const std::string c_name = "";
    ImGuiWindowFlags m_flags = ImGuiWindowFlags_None;
    UIWindowRenderStats m_renderStats = UIWindowRenderStats(c_name + " Render Stats");
    CurrentFrameStats m_currentFrameStats;

private:
    const bool c_autoEndFrame;
  };
}// namespace HummingBirdCore
