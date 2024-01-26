//
//
// Created by Kasper de Bruin on 01/11/2023.
//

#pragma once
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
  };

}// namespace HummingBirdCore