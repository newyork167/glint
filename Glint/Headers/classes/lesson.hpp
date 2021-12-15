class LessonBase {
public:
	virtual ~LessonBase();
	virtual int main_loop();
};

inline LessonBase::~LessonBase() = default;

inline int LessonBase::main_loop() {
	return -1;
}
