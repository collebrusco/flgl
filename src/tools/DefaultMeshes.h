#ifndef DEFAULT_MESHES_H
#define DEFAULT_MESHES_H

#include "../gl_objs/Mesh.h"

struct DefaultMeshes {
	template<typename Vt>
	static Mesh<Vt> tile();
};

#endif
