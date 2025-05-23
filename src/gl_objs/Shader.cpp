//
//  Shader.cpp
//  graphics-library-interface
//
//  Created by Frank Collebrusco on 3/17/23.
//
//

#define GLFW_INCLUDE_GLCOREARB
#define GL_SILENCE_DEPRECATION
#include <sstream>
#include "Shader.h"
#include "../Graphics.h"
#include <flgl/glm.h>
#include "../flgl.h"
#include "../util/log.h"
LOG_MODULE(Shader);

#ifdef DEBUG
static uint32_t numShaders = 0;
#endif

using namespace std;

Shader::Shader(){}

Shader::Shader(uint32_t p) {
    programId = p;
}

Shader::~Shader(){}

void Shader::create(const char* vFileName, const char* fFileName){
    compileAndLink(vFileName, fFileName);
    this->enlist(new Shader(*this));
    LOG_DBG("created %d", programId);
}

Shader Shader::from_source(const char* vFileName, const char* fFileName) {
    Shader s; s.create(vFileName, fFileName); return s;
}

#include <filesystem>
static std::string cpath(std::string p){
   return std::filesystem::current_path().string() + "/" + p;
}

const char* Shader::getShaderSource(string shad, string type){
    ifstream fin;
    string path = glconfig.shader_path() + shad + ".glsl";
    LOG_DBG("looking for shader %s at %s", shad.c_str(), path.c_str());
    fin.open(path);
    if (!fin){
        path = glconfig.flgl_path() + "res/default_shaders/" + shad + ".glsl";
        LOG_DBG("not found, now looking for %s at %s", shad.c_str(), path.c_str());
        fin.open(path);
        if (!fin){
            LOG_ERR("cant find %s shader %s at %s!", type.c_str(), shad.c_str(), cpath(path).c_str());
            return 0;
        }
    }
    
    LOG_INF("found shader %s at %s", shad.c_str(), cpath(path).c_str());

    string contents((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
    fin.close();
    static string src;
    src = contents;
    return src.c_str();
}

bool Shader::check_status(const char* name, GLuint shader) {
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_TRUE){
        LOG_INF("shader %s compiled sucessfully", name);
    } else {
        LOG_ERR("shader %s compilation failed!", name);
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
        std::ostringstream strm;
        strm << "OpenGL Compiler output: \n";
        for (size_t i = 0; i < errorLog.size(); i++){
            strm << errorLog[i];
        }
        LOG_ERR("%s", strm.str().c_str());
        // Exit with failure.
        glDeleteShader(shader); // Don't leak the shader.
        return false;
    }
    return true;
}

bool Shader::compileVertShader(GLuint& vShader, const char* vFileName){
    const char* vSource = getShaderSource(vFileName, "vert");
    vShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vShader, 1, &vSource, NULL);
    glCompileShader(vShader);
    return check_status(vFileName, vShader);
}

bool Shader::compileFragShader(GLuint& fShader, const char* fFileName){
    const char* fSource = getShaderSource(fFileName, "frag");

    fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fSource, NULL);
    glCompileShader(fShader);
    GLint status;
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &status);
    return check_status(fFileName, fShader);
}

bool Shader::linkPrograms(GLuint& vShader, GLuint& fShader, GLuint& prog){
    bool flag = true;
    //create program object
    prog = glCreateProgram();
    //attach vert & frags
    glAttachShader(prog, vShader);
    glAttachShader(prog, fShader);
    //link v&f
    glLinkProgram(prog);
    //error check link
    GLint isLinked = GL_FALSE;
    glGetProgramiv(prog, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE){
        flag = false;
        GLint maxLength = 0;
        glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &maxLength);
        vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(prog, maxLength, &maxLength, &infoLog[0]);
        std::ostringstream strm;
        for (int i = 0; i < maxLength; i++){
            strm << infoLog[i];
        }
        LOG_ERR("Shader link failed!");
        LOG_ERR("OpenGL error output:\n%s", strm.str().c_str());
        //destroy individual shader objs
        glDetachShader(prog, vShader);
        glDetachShader(prog, fShader);
        glDeleteShader(vShader);
        glDeleteShader(fShader);
        glDeleteProgram(prog);
    } else {
        LOG_INF("shader link successful!");
        //destroy individual shader objs
        glDetachShader(prog, vShader);
        glDetachShader(prog, fShader);
        glDeleteShader(vShader);
        glDeleteShader(fShader);

    }
    return flag;
}

bool Shader::compileAndLink(const char* vFileName, const char* fFileName){
    bool flag = true;
    GLuint vShader, fShader;
    if ((!compileVertShader(vShader, vFileName)) || (!compileFragShader(fShader, fFileName))) {
        LOG_ERR("terminating flgl...");
        Graphics::destroy();
        assert(false);
    }
    GLuint prog;
    if (!linkPrograms(vShader, fShader, prog)) {
        LOG_ERR("terminating flgl...");
        Graphics::destroy();
        assert(false);
    }
    Shader::programId = prog;
    return flag;
}

void Shader::bind() const {
    glUseProgram(programId);
}

void Shader::unbind() {
    glUseProgram(0);
}

void Shader::destroy(){
    this->delist();
    LOG_DBG("destroyed %d", programId);
    glDeleteProgram(programId);
}

void Shader::uVec4(const char* varName, const glm::vec4& vec) const{
    bind();
    GLint loc = glGetUniformLocation(programId, varName);
    glUniform4f(loc, vec.x, vec.y, vec.z, vec.w);
}
void Shader::uVec3(const char* varName, const glm::vec3& vec) const{
    bind();
    GLint loc = glGetUniformLocation(programId, varName);
    glUniform3f(loc, vec.x, vec.y, vec.z);
}
void Shader::uVec2(const char* varName, const glm::vec2& vec) const{
    bind();
    GLint loc = glGetUniformLocation(programId, varName);
    glUniform2f(loc, vec.x, vec.y);
}
void Shader::uIVec4(const char* varName, const glm::ivec4& vec) const{
    bind();
    GLint loc = glGetUniformLocation(programId, varName);
    glUniform4i(loc, vec.x, vec.y, vec.z, vec.w);
}
void Shader::uIVec3(const char* varName, const glm::ivec3& vec) const{
    bind();
    GLint loc = glGetUniformLocation(programId, varName);
    glUniform3i(loc, vec.x, vec.y, vec.z);
}
void Shader::uIVec2(const char* varName, const glm::ivec2& vec) const{
    bind();
    GLint loc = glGetUniformLocation(programId, varName);
    glUniform2i(loc, vec.x, vec.y);
}
void Shader::uFloat(const char* varName, const float val) const{
    bind();
    GLint loc = glGetUniformLocation(programId, varName);
    glUniform1f(loc, val);
}
void Shader::uInt(const char* varName, const int val) const{
    bind();
    GLint loc = glGetUniformLocation(programId, varName);
    glUniform1i(loc, val);
}
void Shader::uIntArr(const char* varName, int size, const int* arr) const{
    bind();
    GLint loc = glGetUniformLocation(programId, varName);
    glUniform1iv(loc, size, arr);
}
void Shader::uIntU(const char* varName, uint32_t val) const{
    bind();
    GLint loc = glGetUniformLocation(programId, varName);
    glUniform1i(loc, val);
}
void Shader::uBool(const char* varName, bool val) const{
    bind();
    GLint loc = glGetUniformLocation(programId, varName);
    glUniform1i(loc, val);
}
void Shader::uMat4(const char* varName, const glm::mat4& mat) const{
    bind();
    GLint loc = glGetUniformLocation(programId, varName);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}
void Shader::uMat3(const char* varName, const glm::mat3& mat) const{
    bind();
    GLint loc = glGetUniformLocation(programId, varName);
    glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::uViewProj(glm::mat4 const &v, glm::mat4 const &p, std::string const &vn, std::string const &pn) {
    this->uMat4(vn.c_str(), v);
    this->uMat4(pn.c_str(), p);
}

const GLuint& Shader::programID() const {
    return programId;
}

bool Shader::operator==(const Shader & o) const {
    return o.programID() == this->programID();
}

void Shader::uVec2Arr(const char* varName, int size, const glm::vec2* arr) const {
    bind();
    GLint loc = glGetUniformLocation(programId, varName);
    glUniform2fv(loc, size, glm::value_ptr(arr[0]));
}

void Shader::uVec3Arr(const char* varName, int size, const glm::vec3* arr) const {
    bind();
    GLint loc = glGetUniformLocation(programId, varName);
    glUniform3fv(loc, size, glm::value_ptr(arr[0]));
}

void Shader::uVec4Arr(const char* varName, int size, const glm::vec4* arr) const {
    bind();
    GLint loc = glGetUniformLocation(programId, varName);
    glUniform4fv(loc, size, glm::value_ptr(arr[0]));
}

void Shader::uIVec2Arr(const char* varName, int size, const glm::ivec2* arr) const {
    bind();
    GLint loc = glGetUniformLocation(programId, varName);
    glUniform2iv(loc, size, glm::value_ptr(arr[0]));
}

void Shader::uIVec3Arr(const char* varName, int size, const glm::ivec3* arr) const {
    bind();
    GLint loc = glGetUniformLocation(programId, varName);
    glUniform3iv(loc, size, glm::value_ptr(arr[0]));
}

void Shader::uIVec4Arr(const char* varName, int size, const glm::ivec4* arr) const {
    bind();
    GLint loc = glGetUniformLocation(programId, varName);
    glUniform4iv(loc, size, glm::value_ptr(arr[0]));
}

void Shader::uFloatArr(const char* varName, int size, const float* arr) const {
    bind();
    GLint loc = glGetUniformLocation(programId, varName);
    glUniform1fv(loc, size, arr);
}

void Shader::uMat3Arr(const char* varName, int size, const glm::mat3* arr) const {
    bind();
    GLint loc = glGetUniformLocation(programId, varName);
    glUniformMatrix3fv(loc, size, GL_FALSE, glm::value_ptr(arr[0]));
}

void Shader::uMat4Arr(const char* varName, int size, const glm::mat4* arr) const {
    bind();
    GLint loc = glGetUniformLocation(programId, varName);
    glUniformMatrix4fv(loc, size, GL_FALSE, glm::value_ptr(arr[0]));
}



