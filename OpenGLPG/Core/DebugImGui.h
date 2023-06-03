#pragma once

#include <imgui.h>

#ifdef _DEBUG
#define DEBUG_IMGUI 1
#else
#ifdef NDEBUG
#define DEBUG_IMGUI 0
#endif
#endif
