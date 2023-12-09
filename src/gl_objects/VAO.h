//
//  VAO.h
//  flgl-tester
//
//  Created by Frank Collebrusco on 6/2/23.
//
//

#ifndef VAO_h
#define VAO_h

/*
 TODO: I want to create VAO and VBO abstractions that allow for users to create their own vertex data
 TODO: I like this kind of way but how will the loader know about it?
 TODO: Maybe have these as an internal class to loader? then can call createVBO() to get one
 TODO: then call attrib methods, then call UploadMesh() with it. but this means they'd have to select attribs each time
 TODO: or, this obj only holds data necessary to configure and serves as a template
 TODO: eg, array of attribute structs, which contain data necc to call glVertexAttribPointer
 TODO: then it can be uploaded to loader alongside a pointer, who uses that to configure a vao and buffer
 TODO: meshdetails returned either way. IDK
 TODO: putting this aside for now
 */

//class VAO {
//    
//};
//
//
//class VBO {
// enforce that only the loader can alloc/dealloc vbos
//    void* data;
//    GLuint vbo;
//    VBO();                  // private constructor
//public:
//    void bind() const;
//    void attribute(uint32_t location, GLenum datatype, uint32_t dimension, size_t vertex_size, void* offset);
//};


#endif /* VAO_h */
