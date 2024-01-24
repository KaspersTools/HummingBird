//
//
// Created by Kasper de Bruin on 01/11/2023.
//

#pragma once
#include <PCH/pch.h>

// UIWindows
#include "UIWindows/ContentExplorer.h"
#include "UIWindows/LogWindow.h"
#include "UIWindows/UIWindow.h"

// UIWindows/Themes
#include "UIWindows/Themes/ThemeManager.h"
#include "UIWindows/Themes/Themes.h"

// UIWindows/Widget
#include "UIWindows/Widget/AllAddonsWidget.h"
#include "UIWindows/Widget/DataViewer.h"
#include "UIWindows/Widget/MetricsWidget.h"

// OTHER WINDOWS
#include "Terminal/TerminalWindow.h"

#include "Security/LogInWindow.h"
#include "Security/LoginManager.h"

#include "System/Hosts/EditHostsWindow.h"
#include "System/LaunchDaemonsManager.h"
#include "System/SysInfoWindow.h"


#include "Sql/SqlWindow.h"
#include "Sql/SqlImportWindow.h"

// Rendering
#include "Rendering/Texture.h"

// System
#include "System/SysInfo.h"

// Kasper specific
#ifdef WITHHUMMINGBIRDKASPERSPECIFIC
#include <Hellebrekers/VisualLinkLauncher.h>
#include <HelloLibrary.h>

namespace HummingBirdKasper::VisualLink {
  class VisualLinkLauncher;
}
#endif

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