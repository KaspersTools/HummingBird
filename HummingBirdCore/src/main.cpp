//windows

#include <Application.h>
#include <Log.h>

int main(){
  HummingBirdCore::Log::Init();
  CORE_INFO("Starting application");

  HummingBirdCore::Application app;

  return 0;
}