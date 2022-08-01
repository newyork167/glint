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

// Set your includes here
#include "classes/shader_s.h"
#include "imgui.h"
#include "../external/imgui/imgui_impl_glfw.h"
#include "../external/imgui/imgui_impl_opengl3.h"

#include "boost/filesystem.hpp"
#include <fmt/core.h>

// Local includes
#include "classes/vertex_buffer.hpp"

// Include LearnOpenGL Lessons
#include "../include/learnopengl/0.HelloTriangle/0.HelloTriangle.hpp"

// Define Some Constants
#define PI 3.14159265358979323846
const int m_width = 1280;
const int m_height = 800;

#endif //~ Glint Header
