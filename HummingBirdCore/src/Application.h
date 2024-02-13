//
//
// Created by Kasper de Bruin on 01/11/2023.
//

#pragma once
#include <PCH/pch.h>
#include <HBUI/HBUI.h>

#include "../../HummingBirdPluginManager/include/IPlugin.h"

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

    bool loadPluginManager(const std::filesystem::path &path);

    void render();
    void shutdown();

private:
    HummingBird::Plugins::IPlugin* pluginManager = nullptr;
    void* handle = nullptr;
  };

}// namespace HummingBirdCore