//
// Created by Kasper de Bruin on 01/11/2023.
//

#pragma once

#include <PCH/pch.h>
#include <imgui_internal.h>

// Define a UI window structure
namespace HummingBirdCore {
  struct UIWindowRenderStats{

    static const int HISTORY_SIZE = 100;// Adjust as needed
    float frameRenderTimes[HISTORY_SIZE] = {0};
    float averageFrameTimes[HISTORY_SIZE] = {0};
    float peakFrameTimes[HISTORY_SIZE] = {0};
    int currentFrameIndex = 0;// Index for the current frame in the history arrays

    std::chrono::high_resolution_clock::time_point frameStartTime;
    std::string name;// Name of the window

    float frameRenderTime; // Time taken to render the frame
    long frameCount;       // Total number of frames rendered
    float averageFrameTime;// Average time per frame
    float peakFrameTime;   // Peak frame render time
    float updateRate;      // Update rate of the window

    std::map<std::string, std::shared_ptr<UIWindowRenderStats>> childWindows;
    std::map<std::string, std::string> otherStats;

    UIWindowRenderStats() : name(), frameRenderTime(0),
                            frameCount(0), averageFrameTime(0), peakFrameTime(0), updateRate(0),
                            childWindows({}),
                            frameStartTime(std::chrono::high_resolution_clock::now()) {
    }

    UIWindowRenderStats(const std::string &name) : name(name), frameRenderTime(0),
                                                   frameCount(0), averageFrameTime(0), peakFrameTime(0), updateRate(0),
                                                   childWindows({}),
                                                   frameStartTime(std::chrono::high_resolution_clock::now()) {}


    void startUpdateStats() {
      frameStartTime = std::chrono::high_resolution_clock::now();
    }

    void endUpdateStats() {
      auto end = std::chrono::high_resolution_clock::now();
      std::chrono::duration<float, std::milli> renderDuration = end - frameStartTime;

      frameRenderTime = renderDuration.count();
      frameCount++;
      averageFrameTime = frameRenderTime / frameCount;

      if (frameRenderTime > peakFrameTime) {
        peakFrameTime = frameRenderTime;
      }

      updateRate = 1000.0f / averageFrameTime;

      frameRenderTimes[currentFrameIndex % HISTORY_SIZE] = frameRenderTime;
      averageFrameTimes[currentFrameIndex % HISTORY_SIZE] = averageFrameTime;
      peakFrameTimes[currentFrameIndex % HISTORY_SIZE] = peakFrameTime;

      currentFrameIndex++;
    }

    void addChildWindow(const std::string &name) {
      if(childWindows[name] == nullptr)
        childWindows[name] = std::make_shared<UIWindowRenderStats>(name);
    }

    void addChildWindow(std::shared_ptr<UIWindowRenderStats> window) {
        if(childWindows[window->name] == nullptr)
            childWindows[window->name] = window;
    }

    void showRenderStatsWindow() {
      ImGui::Text("Frame render time: %.3f ms", frameRenderTime);
      ImGui::Text("Frame count: %ld", frameCount);
      ImGui::Text("Average frame time: %.3f ms", averageFrameTime);
      ImGui::Text("Peak frame time: %.3f ms", peakFrameTime);
      ImGui::Text("Update rate: %.3f fps", updateRate);

      ImGui::Separator();
      if (ImGui::CollapsingHeader("Plots")) {

        ImGui::Text("Frame render time: %.3f ms", frameRenderTime);
        ImGui::PlotLines("Frame Render Time", frameRenderTimes, UIWindowRenderStats::HISTORY_SIZE, 0, "Frame Render Time (ms)", 0.0f, *std::max_element(frameRenderTimes, frameRenderTimes + UIWindowRenderStats::HISTORY_SIZE), ImVec2(0, 80));
        ImGui::PlotLines("Average Frame Time", averageFrameTimes, UIWindowRenderStats::HISTORY_SIZE, 0, "Average Frame Time (ms)", 0.0f, *std::max_element(averageFrameTimes, averageFrameTimes + UIWindowRenderStats::HISTORY_SIZE), ImVec2(0, 80));
        ImGui::PlotLines("Peak Frame Time", peakFrameTimes, UIWindowRenderStats::HISTORY_SIZE, 0, "Peak Frame Time (ms)", 0.0f, *std::max_element(peakFrameTimes, peakFrameTimes + UIWindowRenderStats::HISTORY_SIZE), ImVec2(0, 80));
      }

      ImGui::Separator();
      if (ImGui::CollapsingHeader("Other Stats")) {
        for (const auto& stat : otherStats) {
          ImGui::Text("%s: %s", stat.first.c_str(), stat.second.c_str());
        }
      }

      ImGui::Separator();
      if (ImGui::CollapsingHeader("Child Windows")) {
        for (auto &childWindow: childWindows) {
          if (ImGui::TreeNodeEx(childWindow.first.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            childWindow.second->showRenderStatsWindow();
            ImGui::TreePop();
          }
        }
      }
    }

    void addStat(const std::string& id, const std::string& val){
      otherStats[id] = val;
    }

    void addStat(const std::string& id, const int val){
      otherStats[id] = std::to_string(val);
    }

    void addStat(const std::string& id, const float val){
      otherStats[id] = std::to_string(val);
    }
  };

  class UIWindow : public ApplicationObject {
public:
    UIWindow(ImGuiWindowFlags flags, const std::string &name, const bool autoEndFrame = true) : m_flags(flags), m_name(name), c_autoEndFrame(autoEndFrame) {
        m_renderStatsWindowTitle = name + " Render Stats";
        m_renderStats = std::make_shared<UIWindowRenderStats>(m_renderStatsWindowTitle);
    }

    void beginFrame() {
      startUpdateStats();
      if (!m_isOpen) {
        return;
      }

      ImGui::Begin(m_name.c_str(), &m_isOpen, m_flags | ImGuiWindowFlags_MenuBar);
      if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Debug")) {
          if (ImGui::MenuItem("Render Stats")) {
            m_showRenderStats = !m_showRenderStats;
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

      if(!m_isOpen)
      {
        cleanUpOnClose();
      }
    }

    std::string getName() const { return m_name; }
    bool isOpen() const { return m_isOpen; }
    ImGuiWindowFlags getFlags() const { return m_flags; }
    bool getAutoEndFrame() const { return c_autoEndFrame; }

    std::shared_ptr<UIWindowRenderStats> getRenderStats(const std::string& name) {
      if (m_renderStats->name == name) {
        return m_renderStats;
      }

      for (auto &childWindow: m_renderStats->childWindows) {
        if (childWindow.first == name) {
          return childWindow.second;
        }
      }

      return nullptr;
    }

    std::shared_ptr<UIWindowRenderStats> getRenderStats() {
      return m_renderStats;
    }

protected:
    virtual void render() = 0;
    virtual void cleanUpOnClose() {}
    virtual void initialize() {}

private:
    void startUpdateStats() {
      m_renderStats->startUpdateStats();
    }

    void endUpdateStats() {
      m_renderStats->endUpdateStats();
    }

    void showRenderStatsWindow() {
      if (m_showRenderStats) {
        ImGui::Begin(m_renderStats->name.c_str());
        m_renderStats->showRenderStatsWindow();
        ImGui::End();
      }
    }


public:
    bool m_isOpen = true;
    bool m_showRenderStats = false;

    std::string m_name;
    std::string m_renderStatsWindowTitle;

    ImGuiWindowFlags m_flags = ImGuiWindowFlags_None;

    const bool c_autoEndFrame;

private:
    std::shared_ptr<UIWindowRenderStats> m_renderStats = nullptr;

};
}// namespace HummingBirdCore
