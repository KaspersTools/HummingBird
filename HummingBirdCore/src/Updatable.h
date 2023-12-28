//
// Created by Kasper de Bruin on 27/12/2023.
//

#pragma once
namespace HummingBirdCore{
  class Updatable {
    public:
        virtual void update() = 0;
        virtual ~Updatable() = default;
  };
}// namespace HummingBirdCore::Rendering
