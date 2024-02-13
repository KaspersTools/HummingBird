//windows

#include <Application.h>
#include <Log.h>
//#include <HBUI/HBUI.h>
//#include <HBUI/HBUI.h>
int main(){
  HummingBirdCore::Log::Init();
  CORE_INFO("Starting application");

  HummingBirdCore::Application app;

  return 0;
}