#ifndef Shader_h
#define Shader_h

#include <iostream>
#include <fstream>
#include <string>
#include "../gfx.h"
#include "GL_Object.h"
#include <flgl/glm.h>
#include <vector>

class Shader : public GL_Object {
private:
    GLuint programId;
    bool compileFragShader(GLuint& fShader, const char* vFileName);
    bool compileVertShader(GLuint& vShader, const char* fFileName);
    bool linkPrograms(GLuint& vShader, GLuint& fShader, GLuint& prog);
    bool check_status(const char* name, GLuint shader);
    const char* getShaderSource(std::string shad, std::string type);
    bool compileAndLink(const char* vFileName, const char* fFileName);
public:
    Shader();
    ~Shader();
    Shader(uint32_t);
    void create(const char* vFileName, const char* fFileName);
    static Shader from_source(const char* vFileName, const char* fFileName);
    const GLuint& programID() const;
    
    void bind() const;
    static void unbind();
    void destroy() override final;

    void uVec4(const char* varName, const glm::vec4& vec) const;
    void uVec3(const char* varName, const glm::vec3& vec) const;
    void uVec2(const char* varName, const glm::vec2& vec) const;
    void uIVec4(const char* varName, const glm::ivec4& vec) const;
    void uIVec3(const char* varName, const glm::ivec3& vec) const;
    void uIVec2(const char* varName, const glm::ivec2& vec) const;
    void uFloat(const char* varName, const float val) const;
    void uInt(const char* varName, const int val) const;
    void uIntU(const char* varName, uint32_t val) const;
    void uBool(const char* varName, bool val) const;
    void uIntArr(const char* varName, int size, const int* arr) const;
    void uVec2Arr(const char* varName, int size, const glm::vec2* arr) const;
    void uVec3Arr(const char* varName, int size, const glm::vec3* arr) const;
    void uVec4Arr(const char* varName, int size, const glm::vec4* arr) const;
    void uIVec2Arr(const char* varName, int size, const glm::ivec2* arr) const;
    void uIVec3Arr(const char* varName, int size, const glm::ivec3* arr) const;
    void uIVec4Arr(const char* varName, int size, const glm::ivec4* arr) const;
    void uFloatArr(const char* varName, int size, const float* arr) const;
    void uMat3Arr(const char* varName, int size, const glm::mat3* arr) const;
    void uMat4Arr(const char* varName, int size, const glm::mat4* arr) const;

    
    void uMat4(const char* varName, const glm::mat4& mat4) const;
    void uMat3(const char* varName, const glm::mat3& mat4) const;

    void uViewProj(glm::mat4 const& v, glm::mat4 const& p, std::string const& vn = "uView", std::string const& pn = "uProj");
    
    bool operator==(const Shader&) const;
};

#endif /* Shader_h */
