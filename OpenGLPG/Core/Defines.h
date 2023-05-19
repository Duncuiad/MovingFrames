#pragma once

#include <GLFW/glfw3.h>

using GLUID = unsigned int;

constexpr int OPENGL_VERSION_MAJOR = 4;
constexpr int OPENGL_VERSION_MINOR = 5;
constexpr int OPENGL_PROFILE = GLFW_OPENGL_CORE_PROFILE;
constexpr int DEFAULT_WINDOW_WIDTH = 800;
constexpr int DEFAULT_WINDOW_HEIGHT = 600;

// @improvement: remove hardcoded names
constexpr const char* VERTEX_SHADER_DEFAULT_NAME = "vertex_shader_default";
constexpr const char* FRAGMENT_SHADER_DEFAULT_NAME = "fragment_shader_default";
