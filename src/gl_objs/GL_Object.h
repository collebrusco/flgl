#ifndef GL_OBJECT_H
#define GL_OBJECT_H
#include <vector>
using std::vector;

class GL_Object {

	static vector<GL_Object*> objects;

	GL_Object();
	virtual void destroy() = 0;
};

#endif
