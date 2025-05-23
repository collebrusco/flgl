#include "DefaultMeshes.h"
// #include "../util/log.h"
// LOG_MODULE(DefaultMeshes);

/**
 * all of these are CW wound except Vt_pn
 */

template<>
Mesh<Vertex_2f> DefaultMeshes::tile() {
	return Mesh<Vertex_2f>::from_vectors({{-1.,-1.},
									      {-1., 1.},
									      { 1., 1.},
									      { 1.,-1.}}
									      ,
									      {0, 2, 1,	0, 3, 2});
}

template<>
Mesh<Vertex_3f> DefaultMeshes::tile() {
	return Mesh<Vertex_3f>::from_vectors({{-1.,-1., 0.},
									      {-1., 1., 0.},
									      { 1., 1., 0.},
									      { 1.,-1., 0.}}
									      ,
									      {0, 2, 1,	0, 3, 2});
}

template<>
Mesh<Vt_classic> DefaultMeshes::tile() {
	return Mesh<Vt_classic>::from_vectors({{{-1.,-1., 0.}, {0.,0.}},
									    	{{-1., 1., 0.}, {0.,1.}},
									    	{{ 1., 1., 0.}, {1.,1.}},
									    	{{ 1.,-1., 0.}, {1.,0.}}}
									    	,
									    	{0, 2, 1,	0, 3, 2});
}

template<>
Mesh<Vt_2Dclassic> DefaultMeshes::tile() {
	return Mesh<Vt_2Dclassic>::from_vectors({{{-1.,-1.}, {0.,0.}},
									    	 {{-1., 1.}, {0.,1.}},
									    	 {{ 1., 1.}, {1.,1.}},
									    	 {{ 1.,-1.}, {1.,0.}}}
									    	 ,
									    	 {0, 2, 1,	0, 3, 2});
}


template<>
Mesh<Vt_pn> DefaultMeshes::tile() {
	return Mesh<Vt_pn>::from_vectors({{{-0.5,  0.5, 0}, {0,  1, 0}},
								       {{0.5,  0.5, 0}, {0,  1, 0}},   
								       {{0.5,  0.5, 0}, {1,  0, 0}},   
								       {{0.5, -0.5, 0}, {1,  0, 0}},  
								       {{0.5, -0.5, 0}, {0, -1, 0}}, 
								      {{-0.5, -0.5, 0}, {0, -1, 0}},
								      {{-0.5, -0.5, 0}, {-1, 0, 0}},
								      {{-0.5,  0.5, 0}, {-1, 0, 0}}  }
								       ,
								       {0, 1, 2,	2, 3, 4,	4, 5, 6,	6, 7, 0,
								   		0, 2, 6, 	6, 2, 4});
}



