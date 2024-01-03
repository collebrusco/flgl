#ifndef GL_OBJECT_H
#define GL_OBJECT_H
#include <unordered_set>

class GL_Object {
private:
	static std::unordered_set<GL_Object*> objects;
protected:
	GL_Object() = default;
	virtual ~GL_Object() = default; //TODO: log if not empty
	void enlist(GL_Object*);
	void delist();
public:
	virtual void destroy() = 0;
	static void destroy_all();
};

#endif
