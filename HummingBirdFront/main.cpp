//include all if defs f windows __WIN32__ etc
#define WINDOWSBUILD

//windows
#ifdef _WIN32
  #define SDL_MAIN_HANDLED // avoid link error on Linux Windows Subsystem
  #include <SDL.h>
#endif

#include <Application.h>
#include <Log.h>
#include <iostream>



int main() {
  std::cout << "Hello, World!" << std::endl;
  HummingBirdCore::Log::Init();

  HummingBirdCore::Application app;
  return 0;
}
