#pragma once
#define GL_UNBIND 0

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

class ShaderUtilities {
protected:
    double lastTime = glfwGetTime();
    double deltaTime = 0.0f;
    int nbFrames = 0;

    static auto check_number_of_attributes_supported();
    float RandomFloat(float a, float b);

	auto calculate_frame_ms(bool print_to_terminal = true) {
		double currentTime = glfwGetTime();
		nbFrames++;

		// Calculate delta time for physics
		deltaTime = currentTime - lastTime;

		if (deltaTime >= 1.0) { // If last prinf() was more than 1 sec ago
			// printf and reset timer
			if (print_to_terminal)
				std::cout << "\r" << 1000.0 / double(nbFrames) << " ms/frame";
			nbFrames = 0;
			lastTime += 1.0;
		}
	}
};