//
//
// Created by Kasper de Bruin on 01/11/2023.
//

#pragma once
#include "../../HummingBirdTestPlugin/include/IPlugin.h"
#include <PCH/pch.h>

namespace HummingBirdCore {
  class Application {
public:
    Application(){
      init();
    }
    ~Application(){
      shutdown();
    }
    bool run();
private:
    void init();
    void render();
    void shutdown();

private:
    bool showDebugWindow = false;
    HummingBird::Plugins::IPlugin* plugin = nullptr;
    void* handle = nullptr;
  };

}// namespace HummingBirdCore