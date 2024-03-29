// Preprocessor Directives
#ifndef GLINT
#define GLINT
#pragma once

// System Headers
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <btBulletDynamicsCommon.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// Reference: https://github.com/nothings/stb/blob/master/stb_image.h#L4
// To use stb_image, add this in *one* C++ source file.
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// STD Includes
#include <vector>
#include <numeric>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/async_logger.h>
#include <spdlog/async.h>
#include <time.h>

// Set your includes here
#include "classes/shader_s.h"
#include "imgui.h"
#include "../external/imgui/imgui_impl_glfw.h"
#include "../external/imgui/imgui_impl_opengl3.h"

#include "boost/filesystem.hpp"
#include <fmt/core.h>

// Local includes

// Include LearnOpenGL Lessons
#include "../include/learnopengl/0.HelloTriangle/0.HelloTriangle.hpp"

// Define Some Constants
#define PI 3.14159265358979323846
const int m_width = 1280;
const int m_height = 800;

inline std::vector<Shader *> program_shaders{};
inline time_t start = time(nullptr);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

#endif //~ Glint Header
