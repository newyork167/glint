#include "shader_s.h"

class LessonBase {
public:
	virtual ~LessonBase();
	virtual int main_loop();

protected:
    const int m_width = 1280;
    const int m_height = 800;
};

inline LessonBase::~LessonBase() = default;

inline int LessonBase::main_loop() {
	return -1;
}
