#include "classes/lesson.hpp"

#include <iostream>
#include <ostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Lesson : public LessonBase {
public:
	int main_loop() override;

private:
    int build_triangle(unsigned int *VAO, unsigned int *VBO, unsigned int *EBO);
};
