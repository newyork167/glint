#include "learnopengl/0.GettingStarted/0.CreatingAWindow.hpp"

int Lesson::main_loop() {
	// Load GLFW and Create a Window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	auto m_window = glfwCreateWindow(m_width, m_height, "OpenGL", nullptr, nullptr);

	// Check for Valid Context
	if (m_window == nullptr) {
		fprintf(stderr, "Failed to Create OpenGL Context");
		return EXIT_FAILURE;
	}

	// Create Context and Load OpenGL Functions
	glfwMakeContextCurrent(m_window);
	gladLoadGL();
	std::cerr << "OpenGL " << glGetString(GL_VERSION) << std::endl;

	// Turn off v-sync
	glfwSwapInterval(0);

	// Rendering Loop
	while (glfwWindowShouldClose(m_window) == false) {
		if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(m_window, true);

		// Background Fill Color
		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Flip Buffers and Draw
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}   glfwTerminate();
	return EXIT_SUCCESS;
}
