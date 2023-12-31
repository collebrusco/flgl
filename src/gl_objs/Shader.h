#ifndef Shader_h
#define Shader_h

#include <iostream>
#include <fstream>
#include <string>
#include "../gfx.h"
#include "GL_Object.h"
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
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
    void unbind() const;
    void destroy() override;

    void uVec4(const char* varName, const glm::vec4& vec) const;
    void uVec3(const char* varName, const glm::vec3& vec) const;
    void uVec2(const char* varName, const glm::vec2& vec) const;
    void uIVec4(const char* varName, const glm::ivec4& vec) const;
    void uIVec3(const char* varName, const glm::ivec3& vec) const;
    void uIVec2(const char* varName, const glm::ivec2& vec) const;
    void uFloat(const char* varName, const float val) const;
    void uInt(const char* varName, const int val) const;
    void uIntArr(const char* varName, int size, const int* arr) const;
    void uIntU(const char* varName, uint32_t val) const;
    void uBool(const char* varName, bool val) const;
    
    void uMat4(const char* varName, const glm::mat4& mat4) const;
    void uMat3(const char* varName, const glm::mat3& mat4) const;
    
    bool operator==(const Shader&) const;
};

#endif /* Shader_h */
