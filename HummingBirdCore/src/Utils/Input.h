//
// Created by Kasper de Bruin on 20/12/2023.
//
#pragma once

#include "Log.h"
#include <SDL.h>
#include <vector>

//TODO: expand this class
// with mouse locations etc.
namespace HummingBirdCore {
  //Class is fully static
  class Input {
public:
    inline static void update(const SDL_Event &event) {
      //Keys
      if (event.type == SDL_KEYDOWN) {
        m_isAnyKeyPressed = true;
        if (event.key.keysym.mod & KMOD_LSHIFT || event.key.keysym.mod & KMOD_RSHIFT) {
          m_isAnyShiftPressed = true;
          m_isLeftShiftPressed = true;
        }
        if (event.key.keysym.mod & KMOD_LALT || event.key.keysym.mod & KMOD_RALT) {
          m_isAnyAltPressed = true;
          m_isLeftAltPressed = true;
        }
        if (event.key.keysym.mod & KMOD_LCTRL || event.key.keysym.mod & KMOD_RCTRL) {
          m_isAnyCtrlPressed = true;
          m_isLeftCtrlPressed = true;
        }
      }
      if (event.type == SDL_KEYUP) {
        if (!(event.key.keysym.mod & KMOD_LSHIFT || event.key.keysym.mod & KMOD_RSHIFT)) {
          m_isAnyShiftPressed = false;
          m_isLeftShiftPressed = false;
        }
        if (!(event.key.keysym.mod & KMOD_LALT || event.key.keysym.mod & KMOD_RALT)) {
          m_isAnyAltPressed = false;
          m_isLeftAltPressed = false;
        }
        if (!(event.key.keysym.mod & KMOD_LCTRL || event.key.keysym.mod & KMOD_RCTRL)) {
          m_isAnyCtrlPressed = false;
          m_isLeftCtrlPressed = false;
        }
      }

      if (event.type == SDL_KEYUP) {
        for (int i = 0; i < m_pressedKeys.size(); i++) {
          if (m_pressedKeys[i] == event.key.keysym.sym) {
            m_pressedKeys.erase(m_pressedKeys.begin() + i);
            break;
          }
        }
      }

      if (event.type == SDL_KEYDOWN) {
        bool add = true;
        if (m_pressedKeys.size() > 0) {
          for (int i = 0; i < m_pressedKeys.size(); i++) {
            if (m_pressedKeys[i] == event.key.keysym.sym) {
              add = false;
              break;
            }
          }
        }
        if (add) {
          m_pressedKeys.push_back(event.key.keysym.sym);
        }
      }

      if (m_pressedKeys.size() > 0) {
        m_isAnyKeyPressed = true;
      }

      //Mouse
      if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
          m_isLeftMousePressed = true;
        }
        if (event.button.button == SDL_BUTTON_RIGHT) {
          m_isRightMousePressed = true;
        }
        if (event.button.button == SDL_BUTTON_MIDDLE) {
          m_isMiddleMousePressed = true;
        }
      }

      if (event.type == SDL_MOUSEBUTTONUP) {
        if (event.button.button == SDL_BUTTON_LEFT) {
          m_isLeftMousePressed = false;
        }
        if (event.button.button == SDL_BUTTON_RIGHT) {
          m_isRightMousePressed = false;
        }
        if (event.button.button == SDL_BUTTON_MIDDLE) {
          m_isMiddleMousePressed = false;
        }
      }
    }

    inline static bool isKeyPressed (SDL_Keycode key) {
      for (int i = 0; i < m_pressedKeys.size(); i++) {
        if (m_pressedKeys[i] == key) {
          return true;
        }
      }
      return false;
    }

    inline static bool isAnyKeyPressed() {
      return m_isAnyKeyPressed;
    }

    inline static bool isAnyShiftPressed() {
      return m_isAnyShiftPressed;
    }

    inline static bool isAnyAltPressed() {
      return m_isAnyAltPressed;
    }

    inline static bool isAnyCtrlPressed() {
      return m_isAnyCtrlPressed;
    }

    inline static bool isLeftShiftPressed() {
      return m_isLeftShiftPressed;
    }

    inline static bool isLeftAltPressed() {
      return m_isLeftAltPressed;
    }

    inline static bool isLeftCtrlPressed() {
      return m_isLeftCtrlPressed;
    }

    inline static bool isLeftMousePressed() {
      return m_isLeftMousePressed;
    }

    inline static bool isRightMousePressed() {
      return m_isRightMousePressed;
    }

    inline static bool isMiddleMousePressed() {
      return m_isMiddleMousePressed;
    }

    //
    inline static void reset() {
      m_isAnyKeyPressed = false;
      m_isAnyShiftPressed = false;
      m_isAnyAltPressed = false;
      m_isAnyCtrlPressed = false;
      m_isLeftShiftPressed = false;
      m_isLeftAltPressed = false;
      m_isLeftCtrlPressed = false;
      m_isLeftMousePressed = false;
      m_isRightMousePressed = false;
      m_isMiddleMousePressed = false;
      m_pressedKeys.clear();
    }

private:
    //keys
    //Inputs
    inline static bool m_isAnyShiftPressed = false;
    inline static bool m_isAnyAltPressed = false;
    inline static bool m_isAnyKeyPressed = false;
    inline static bool m_isAnyCtrlPressed = false;
    //left
    inline static bool m_isLeftCtrlPressed = false;
    inline static bool m_isLeftShiftPressed = false;
    inline static bool m_isLeftAltPressed = false;
    //Right
    inline static bool m_isRightCtrlPressed = false;
    inline static bool m_isRightShiftPressed = false;
    inline static bool m_isRightAltPressed = false;
    //other keys
    inline static std::vector<SDL_Keycode> m_pressedKeys;

    //Mouse
    inline static bool m_isLeftMousePressed = false;
    inline static bool m_isRightMousePressed = false;
    inline static bool m_isMiddleMousePressed = false;
  };
}// namespace HummingBirdCore
