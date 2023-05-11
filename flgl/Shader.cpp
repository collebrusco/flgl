//
//  Shader.cpp
//  graphics-library-interface
//
//  Created by Frank Collebrusco on 3/17/23.
//
//

#define GLFW_INCLUDE_GLCOREARB
#define GL_SILENCE_DEPRECATION
#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>

#ifdef DEBUG
static uint32_t numShaders = 0;
#endif

Shader::Shader(){}

Shader::~Shader(){}

Shader::Shader(const char* vFileName, const char* fFileName){
    compileAndLink(vFileName, fFileName);
}

const char* Shader::getShaderSource(string shad, string type){
    ifstream fin;
    string path = "gfx/Shaders/" + shad + ".glsl";
    cout << "Reading shader at: " << path << endl;
    fin.open(path);
    if (!fin){
        cout << "error opening " + type + " shader!\n";
    }
    string contents((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
    fin.close();
    static string src;
    src = contents;
    return src.c_str();
}

bool Shader::compileVertShader(GLuint& vShader, const char* vFileName){
    const char* vSource = getShaderSource(vFileName, "vert");
    vShader = glCreateShader(GL_VERTEX_SHADER);
    #ifdef DEBUG
    numShaders++;
    #endif
    glShaderSource(vShader, 1, &vSource, NULL);
    glCompileShader(vShader);
    GLint status;
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &status);
    if (status == GL_TRUE){
        cout << vFileName << " shader compiled sucessfully!" << endl;
    } else {
        cout << vFileName << " shader compilation failed!" << endl;
        GLint maxLength = 0;
        glGetShaderiv(vShader, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(vShader, maxLength, &maxLength, &errorLog[0]);
        cout << "Error: \n";
        for (int i = 0; i < errorLog.size(); i++){
            cout << errorLog[i];
        }
        cout << "\n";
        // Exit with failure.
        #ifdef DEBUG
        numShaders--;
        #endif
        glDeleteShader(vShader); // Don't leak the shader.
        return false;
    }
    return true;
}

bool Shader::compileFragShader(GLuint& fShader, const char* fFileName){
    const char* fSource = getShaderSource(fFileName, "frag");

    fShader = glCreateShader(GL_FRAGMENT_SHADER);
    #ifdef DEBUG
    numShaders++;
    #endif
    glShaderSource(fShader, 1, &fSource, NULL);
    glCompileShader(fShader);
    GLint status;
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &status);
    if (status == GL_TRUE){
        cout << fFileName << " shader compiled sucessfully!" << endl;
    } else {
        cout << fFileName << " shader compilation failed!" << endl;
        GLint maxLength = 0;
        glGetShaderiv(fShader, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(fShader, maxLength, &maxLength, &errorLog[0]);
        cout << "Error: \n";
        for (int i = 0; i < errorLog.size(); i++){
            cout << errorLog[i];
        }
        cout << "\n";
        // Exit with failure.
        glDeleteShader(fShader); // Don't leak the shader.
        #ifdef DEBUG
        numShaders--;
        #endif
        return false;
    }
    return true;
}

bool Shader::linkPrograms(GLuint& vShader, GLuint& fShader, GLuint& prog){
    bool flag = true;
    //create program object
    prog = glCreateProgram();
    #ifdef DEBUG
    numShaders++;
    #endif
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
        for (int i = 0; i < maxLength; i++){
            cout << infoLog[i];
        }
        cout << endl;
        //destroy individual shader objs
        glDetachShader(prog, vShader);
        glDetachShader(prog, fShader);
        glDeleteShader(vShader);
        glDeleteShader(fShader);
        glDeleteProgram(prog);
        #ifdef DEBUG
        numShaders-=3;
        #endif
    } else {
        cout << "Shader link successful!\n";
        //destroy individual shader objs
        glDetachShader(prog, vShader);
        glDetachShader(prog, fShader);
        glDeleteShader(vShader);
        glDeleteShader(fShader);
        #ifdef DEBUG
        numShaders-=2;
        #endif
    }
    return flag;
}

bool Shader::compileAndLink(const char* vFileName, const char* fFileName){
    bool flag = true;
    GLuint vShader, fShader;
    assert(compileVertShader(vShader, vFileName));
    assert(compileFragShader(fShader, fFileName));

    GLuint prog;
    linkPrograms(vShader, fShader, prog);
    Shader::programId = prog;
    
    return flag;
}

void Shader::bind() const {
    glUseProgram(programId);
}

void Shader::unBind() const {
    glUseProgram(0);
}

// if this is called after program has already been destroyed, is there a problem?
void Shader::destroy(){
    #ifdef DEBUG
    cout << "shader deleted, " << --numShaders << " remain..\n";
    #endif
    glDeleteProgram(programId);
}

void Shader::uVec4(const char* varName, const glm::vec4& vec) const{
    GLint loc = glGetUniformLocation(programId, varName);
    glUniform4f(loc, vec.x, vec.y, vec.z, vec.w);
}
void Shader::uVec3(const char* varName, const glm::vec3& vec) const{
    GLint loc = glGetUniformLocation(programId, varName);
    glUniform3f(loc, vec.x, vec.y, vec.z);
}
void Shader::uVec2(const char* varName, const glm::vec2& vec) const{
    GLint loc = glGetUniformLocation(programId, varName);
    glUniform2f(loc, vec.x, vec.y);
}
void Shader::uIVec4(const char* varName, const glm::ivec4& vec) const{
        GLint loc = glGetUniformLocation(programId, varName);
        glUniform4i(loc, vec.x, vec.y, vec.z, vec.w);
}
void Shader::uIVec3(const char* varName, const glm::ivec3& vec) const{
    GLint loc = glGetUniformLocation(programId, varName);
    glUniform3i(loc, vec.x, vec.y, vec.z);
}
void Shader::uIVec2(const char* varName, const glm::ivec2& vec) const{
    GLint loc = glGetUniformLocation(programId, varName);
    glUniform2i(loc, vec.x, vec.y);
}
void Shader::uFloat(const char* varName, const float val) const{
    GLint loc = glGetUniformLocation(programId, varName);
    glUniform1f(loc, val);
}
void Shader::uInt(const char* varName, const int val) const{
    GLint loc = glGetUniformLocation(programId, varName);
    glUniform1i(loc, val);
}
void Shader::uIntArr(const char* varName, int size, const int* arr) const{
    GLint loc = glGetUniformLocation(programId, varName);
    glUniform1iv(loc, size, arr);
}
void Shader::uIntU(const char* varName, uint32_t val) const{
    GLint loc = glGetUniformLocation(programId, varName);
    glUniform1i(loc, val);
}
void Shader::uBool(const char* varName, bool val) const{
    GLint loc = glGetUniformLocation(programId, varName);
    glUniform1i(loc, val);
}
void Shader::uMat4(const char* varName, const glm::mat4& mat) const{
    GLint loc = glGetUniformLocation(programId, varName);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}
void Shader::uMat3(const char* varName, const glm::mat3& mat) const{
    GLint loc = glGetUniformLocation(programId, varName);
    glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}

const GLuint& Shader::programID() const {
    return programId;
}
