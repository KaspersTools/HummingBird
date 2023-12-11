//
// Created by Kasper de Bruin on 10/12/2023.
//

#ifndef KBTOOLS_EDITHOSTS_H
#define KBTOOLS_EDITHOSTS_H
#include <iostream>

#include "../../UIWindows/UIWindow.h"

namespace HummingBird::System {
  class EditHosts : public KBTools::UIWindow{
    public:
      EditHosts(const std::string &filelocation) : EditHosts(ImGuiWindowFlags_None, filelocation) {}
      EditHosts(const ImGuiWindowFlags flags, const std::string &filelocation);
      ~EditHosts();

      void Render() override;
  private:
      const std::string& c_filelocation;
      std::string m_filecontent;
  };
}// namespace HummingBird::System

#endif//KBTOOLS_EDITHOSTS_H
