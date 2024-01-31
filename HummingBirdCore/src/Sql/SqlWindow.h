//
// Created by Kasper de Bruin on 31/01/2024.
//

#ifndef HUMMINGBIRD_SQLWINDOW_H
#define HUMMINGBIRD_SQLWINDOW_H

#include <UIWindows/UIWindow.h>

namespace HummingBirdCore {
  class SqlWindow : public UIWindow {
  public:
    SqlWindow(std::string name) : UIWindow(name) {
    }

    void render() override;
  };
}
class SqlWindow {
};


#endif//HUMMINGBIRD_SQLWINDOW_H
