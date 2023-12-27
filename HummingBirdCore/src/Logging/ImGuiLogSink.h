//
// Created by Kasper de Bruin on 26/12/2023.
//

#pragma once
#include <fmt/format.h>
#include <iostream>
#include <mutex>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/base_sink.h>
#include <string>
#include <vector>

namespace HummingBirdCore::Logging {
  struct ImGuiLogSinkItem {
    ImGuiLogSinkItem(const std::string &message, spdlog::level::level_enum level) : message(message), level(level) {}
    std::string message;
    spdlog::level::level_enum level;
  };

  template<typename Mutex>
  class ImGuiLogSink : public spdlog::sinks::base_sink<Mutex> {
protected:
    void sink_it_(const spdlog::details::log_msg &msg) override {
      spdlog::memory_buf_t formatted;
      spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
      spdlog::level::level_enum level = msg.level;
      std::string message = fmt::to_string(formatted);
      AddLog(ImGuiLogSinkItem(message, level));
    }

    void flush_() override {
      // Flush method
    }

public:
    explicit ImGuiLogSink() {
      if(s_Instance != nullptr){
        CORE_ERROR("ImGuiLogSink already initialized, not initializing again. Make sure to only initialize ImGuiLogSink once.!!");
        return;
      }

      s_Instance = this;
    }

    static ImGuiLogSink* getInstance(){
      if(s_Instance == nullptr){
        CORE_ERROR("ImGuiLogSink not initialized, initializing now. Make sure to initialize ImGuiLogSink before using it.!!");
        s_Instance = new ImGuiLogSink();
      }
      return s_Instance;
    }

    std::vector<ImGuiLogSinkItem> getLogMessages() const {
      return m_logMessages;
    }

private:
    mutable std::vector<ImGuiLogSinkItem> m_logMessages;

    void AddLog(const ImGuiLogSinkItem &item) {
      m_logMessages.push_back(item);
    }

    static ImGuiLogSink* s_Instance;
  };

  template<typename Mutex>
  ImGuiLogSink<Mutex>* ImGuiLogSink<Mutex>::s_Instance = nullptr;

  using ImGuiLogSink_mt = ImGuiLogSink<std::mutex>;// Thread-safe version
}// namespace HummingBirdCore::Logging