#include "classes/lesson.hpp"

#include <iostream>
#include <ostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Lesson : public LessonBase {
public:
	int main_loop() override;

private:
	const int m_width = 1280;
	const int m_height = 800;
};
