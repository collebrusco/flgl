#include "DefaultMeshes.h"
// #include "../util/log.h"
// LOG_MODULE(DefaultMeshes);

template<>
Mesh<Vertex_2f> DefaultMeshes::tile() {
	return Mesh<Vertex_2f>::from_vectors({{-1.,-1.},
									      {-1., 1.},
									      { 1., 1.},
									      { 1.,-1.}}
									      ,
									      {0, 2, 1,	0, 2, 3});
}

template<>
Mesh<Vertex_3f> DefaultMeshes::tile() {
	return Mesh<Vertex_3f>::from_vectors({{-1.,-1., 0.},
									      {-1., 1., 0.},
									      { 1., 1., 0.},
									      { 1.,-1., 0.}}
									      ,
									      {0, 2, 1,	0, 2, 3});
}

template<>
Mesh<Vt_classic> DefaultMeshes::tile() {
	return Mesh<Vt_classic>::from_vectors({{{-1.,-1., 0.}, {0.,0.}},
									    	{{-1., 1., 0.}, {0.,1.}},
									    	{{ 1., 1., 0.}, {1.,1.}},
									    	{{ 1.,-1., 0.}, {1.,0.}}}
									    	,
									    	{0, 2, 1,	0, 2, 3});
}

// template<>
// Mesh<VT_pun> DefaultMeshes::tile() {
// 	return Mesh<VT_pun>::from_vectors({{{-1.,-1., 0.}, {0.,0.}},		// left
// 								       {{-1., 1., 0.}, {0.,1.}},
// 								       {{ 1., 1., 0.}, {1.,1.}},
// 								       {{ 1.,-1., 0.}, {1.,0.}}}
// 								       ,
// 								       {0, 2, 1,	0, 2, 3});
// }


/*
	 _
	|X|
	 -
*/



