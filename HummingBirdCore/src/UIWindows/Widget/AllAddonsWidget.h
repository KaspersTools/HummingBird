//
// Created by Kasper de Bruin on 15/01/2024.
//

#ifndef HUMMINGBIRD_ALLADDONSWIDGET_H
#define HUMMINGBIRD_ALLADDONSWIDGET_H

#include <PCH/pch.h>
#include <addons/imguidatechooser/imguidatechooser.h>
#include <time.h>

namespace HummingBirdCore {
  class AddonWidget : public UIWindow {
public:
    explicit AddonWidget(const std::string &name) : UIWindow(name, ImGuiWindowFlags_MenuBar, false, false) {
    }
    ~AddonWidget() = default;
    void render() override {
      ImGui::Begin("Addon Widget");
      if(ImGui::Button("Show/Hide date chooser")){
        m_showDateChooser = !m_showDateChooser;
      }
      ImGui::End();


      if(m_showDateChooser)
        showDateChooser();

    }

    void showDateChooser() {
      ImGui::TestDateChooser();
    }

private:
    bool m_showDateChooser = false;
  };
}// namespace HummingBirdCore

#endif//HUMMINGBIRD_ALLADDONSWIDGET_H
