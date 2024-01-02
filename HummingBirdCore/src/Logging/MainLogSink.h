//
// Created by Kasper de Bruin on 29/12/2023.
//

#pragma once
#include <PCH/pch.h>

namespace HummingBirdCore {
  namespace Logging {
    template<typename Mutex>
    class MainLogSink : public ImGuiLogSink<std::mutex> {
      public:
          static MainLogSink* getInstance(){
            if(s_Instance == nullptr){
              CORE_ERROR("ImGuiLogSink not initialized, initializing now. Make sure to initialize ImGuiLogSink before using it.!!");
              s_Instance = new MainLogSink();
            }
            return s_Instance;
          }

      private:
          static MainLogSink* s_Instance;
    };

    template<typename Mutex>
    MainLogSink<Mutex>* MainLogSink<Mutex>::s_Instance = nullptr;

    using MainLogSink_mt = MainLogSink<std::mutex>;// Thread-safe version
  }// namespace Logging
}// namespace HummingBirdCore

