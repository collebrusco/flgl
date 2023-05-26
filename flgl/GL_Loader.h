//
//  GL_Loader.h
//  flgl
//
//  Created by Frank Collebrusco on 5/16/23.
//
//  GL_Loader is responsible for every operation that involves loading and unloading to the GPU
//  The loader maintains a list of everything that's uploaded and deallocates all on destroy();
//  The loader is static, as there is only one representation of the GPU's state
//
//  1.  Set relative path to your shader and texture directories
//
//  2.  Upload/Unload Shaders
//          Uploading will compile and link shaders. If there are errors, the program will stop and print them
//          Uploading returns an instance of Shader, which handles binding and uploading variables
//          use Shaders(); to iterate through all shaders, useful when data needs to be uploaded to all of them
//
//  3.  Upload/Unload Textures
//          Uploading returns a TEXTURE_SLOT, which can be uploaded to shaders to select and use the texture
//
//  4. Upload/Unload Meshes
//          Upload Meshes, which are arrays of verticies and elements. See Vertex.h for details on Mesh objects
//          Meshes.h contains several included meshes, a tile and a cube. Upload them if you want

#ifndef GL_Loader_h
#define GL_Loader_h
#include <unordered_set>
#include <string>
#include "Shader.h"
#include "Vertex.h"

typedef uint32_t TEXTURE_SLOT;
typedef uint32_t VAO;

namespace std {
template <> struct hash<Shader> {
    size_t operator()(Shader const& shad) const {
        return std::hash<uint32_t>{}(shad.programID());
    }
};
}

class GL_Loader {
private:
    static TEXTURE_SLOT slotsInUse;
    static std::unordered_set<TEXTURE_SLOT> textures;
    static std::unordered_set<VAO> VAOs;
    static std::unordered_set<Shader> shaders;
    static std::string asset_path;
public:
    static void setAssetPath(std::string path);
    static void setShaderPath(std::string path);
    
    static Shader UploadShader(std::string vert, std::string frag);
    static void UnloadShader(Shader& shad);
    static std::unordered_set<Shader>& Shaders();
    
    static TEXTURE_SLOT UploadTexture(std::string name, bool pixelated);
    static MeshDetails UploadMesh(const ConstMesh& mesh);
    static MeshDetails UploadMesh(Mesh const& mesh);

    static void UnloadMesh(VAO& vao);
    static void UnloadMesh(MeshDetails&);
    static void UnloadTexture(TEXTURE_SLOT);
    
    static void destroy();
};

#endif /* GL_Loader_h */
