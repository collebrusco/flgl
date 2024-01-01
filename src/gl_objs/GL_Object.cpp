#include "GL_Object.h"
using std::unordered_set;
#include "../util/log.h"
LOG_MODULE(GL_Obj);

unordered_set<GL_Object*> GL_Object::objects;

void GL_Object::enlist(GL_Object* ob) {
	objects.insert(ob);
}

void GL_Object::delist() {
	// objects.erase(this);
}

void GL_Object::destroy_all() {
	unordered_set<GL_Object*> set_copy = objects;
	for (GL_Object* obj : set_copy) {
		LOG_DBG("destroying %p", obj);
		obj->destroy();
		delete obj;
	}
	objects.clear();
}
