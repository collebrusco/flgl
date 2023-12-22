#include "GL_Object.h"
#include "../GL_Loader.h"

vector<GL_Object*> GL_Object::objects;

GL_Object::GL_Object() {
	objects.push_back(this);
}