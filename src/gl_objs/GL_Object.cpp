#include "GL_Object.h"
using std::unordered_set;

unordered_set<GL_Object*> GL_Object::objects;

void GL_Object::enlist() {
	objects.insert(this);
}

void GL_Object::delist() {
	objects.erase(this);
}

void GL_Object::destroy_all() {
	unordered_set<GL_Object*> set_copy(objects);
	for (GL_Object* obj : set_copy) {
		obj->destroy();
	}
	objects.clear();
}
