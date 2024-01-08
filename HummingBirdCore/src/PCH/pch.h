//
// Created by Kasper de Bruin on 02/01/2024.
//

//Contains all the precompiled headers for the HummingBirdCore project that are universal for all platforms
#pragma once

/////STANDARD LIBRARIES/////
#include <iostream>
#include <fstream>
#include <sstream>
#include <ostream>

#include <vector>
#include <string>
#include <map>
#include <unordered_map>

#include <memory>

#include <algorithm>
#include <functional>
#include <utility>

#include <thread>
#include <mutex>

#include <chrono>

#include <stdexcept>
#include <filesystem>

#include <cmath>
#include <numeric>
#include <limits>

#include <cstring>
#include <cctype>

#include <cstdlib>
#include <ctime>

#include <type_traits>
#include <typeinfo>

#include <regex>

#ifdef __APPLE__
#include <pwd.h>
#endif

/////HUMMINGBIRD CORE/////
//Contains all the precompiled headers for the HummingBirdCore project that are universal for all platforms
/////THIRD PARTY LIBRARIES/////
#include "SDL_opengl.h"
#include <SDL.h>

#define IM_VEC2_CLASS_EXTRA
#include <imconfig.h>
#include <misc/cpp/imgui_stdlib.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <addons/imguinotify/imgui_notify.h>

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

#include <fmt/core.h>
#include <mysql.h>

#include <Plist.hpp>
#include <boost/any.hpp>

/////HUMMINGBIRD CORE/////
//main
#include "Log.h"

//utils
#include "Utils/Utils.h"
#include "Utils/UUID.h"
#include "Utils/Input.h"
#include "Utils/UIUtils.h"

//rendering
#include "Rendering/Texture.h"

#include "CoreRef.h"
#include "Updatable.h"
#include "ApplicationObject.h"

//helper
#include "Folder.h"

//sql
#include "Sql/SqlConnection.h"



