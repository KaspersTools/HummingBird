#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Application.h"

#include "Sql/SqlConnection.h"
#include "Log.h"

using namespace KBTools;
using namespace HummingBird::Sql;

int main(){
  //Always initialize the logger first
  Log::Init();
  //Run the application
  KBTools::Application app;
}