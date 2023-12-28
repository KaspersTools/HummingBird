//windows
#ifdef _WIN32
  #define SDL_MAIN_HANDLED // avoid link error on Linux Windows Subsystem
  #include <SDL.h>
#endif

#include <Application.h>
#include <Log.h>

int main() {
  HummingBirdCore::Log::Init();

  HummingBirdCore::Application app;
  return 0;
}
