#pragma once

#include <imgui.h>

#ifdef _DEBUG
#define EDITOR_IMGUI 1
#else
#ifdef NDEBUG
#define EDITOR_IMGUI 0
#endif
#endif
