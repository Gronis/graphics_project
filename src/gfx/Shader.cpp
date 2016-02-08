#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>

using namespace engine::gfx;

Shader::Shader(const std::string& shaderCode, GLenum shaderType) :
ref_(0)
{
    //create the shader ref
    ref_ = glCreateShader(shaderType);
    if(ref_ == 0)
        throw std::runtime_error("glCreateShader failed");
    
    //set the source code
    const char* code = shaderCode.c_str();
    glShaderSource(ref_, 1, (const GLchar**)&code, NULL);
    
    //compile
    glCompileShader(ref_);
    
    //throw exception if compile error occurred
    GLint status;
    glGetShaderiv(ref_, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        std::string msg("Compile failure in shader:\n");
        
        GLint infoLogLength;
        glGetShaderiv(ref_, GL_INFO_LOG_LENGTH, &infoLogLength);
        char* strInfoLog = new char[infoLogLength + 1];
        glGetShaderInfoLog(ref_, infoLogLength, NULL, strInfoLog);
        msg += strInfoLog;
        delete[] strInfoLog;
        
        glDeleteShader(ref_); ref_ = 0;
        throw std::runtime_error(msg);
    }
}
Shader::~Shader(){}

ShaderProgram::ShaderProgram(std::initializer_list<Shader> shaders)  :
ref_(0)
{
    if(shaders.size() <= 0)
        throw std::runtime_error("No shaders were provided to create the program");

    //create the program ref
    ref_ = glCreateProgram();
    if(ref_ == 0){
        throw std::runtime_error("glCreateProgram failed");
    }
    //attach all the shaders
    for(auto && shader : shaders){
        glAttachShader(ref_, shader.ref());
    }
    //link the shaders together
    glLinkProgram(ref_);

    //detach all the shaders
  for(auto && shader : shaders){
        glDetachShader(ref_, shader.ref());
    }
    //throw exception if linking failed
    GLint status;
    glGetProgramiv(ref_, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        std::string msg("ShaderProgram linking failure: ");

        GLint infoLogLength;
        glGetProgramiv(ref_, GL_INFO_LOG_LENGTH, &infoLogLength);
        char* strInfoLog = new char[infoLogLength + 1];
        glGetProgramInfoLog(ref_, infoLogLength, NULL, strInfoLog);
        msg += strInfoLog;
        delete[] strInfoLog;

        glDeleteProgram(ref_); ref_ = 0;
        throw std::runtime_error(msg);
    }
}


ShaderProgram::~ShaderProgram() {
    //might be 0 if ctor fails by throwing exception
    if(ref_ != 0) glDeleteProgram(ref_);
}

GLuint ShaderProgram::ref() const {
    return ref_;
}

void ShaderProgram::use() const {
    glUseProgram(ref_);
}

bool ShaderProgram::isInUse() const {
    GLint currentProgram = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    return (currentProgram == (GLint)ref_);
}

void ShaderProgram::stopUsing() const {
    assert(isInUse());
    glUseProgram(0);
}

GLint ShaderProgram::attrib(const GLchar* attribName) const {
    if(!attribName){
        throw std::runtime_error("attribName was NULL");
    }
    GLint attrib = glGetAttribLocation(ref_, attribName);
    if(attrib == -1){
        throw std::runtime_error(std::string("ShaderProgram attribute not found: ") + attribName);
    }
    return attrib;
}

GLint ShaderProgram::uniform(const GLchar* uniformName) const {
    if(!uniformName){
        throw std::runtime_error("uniformName was NULL");
    }
    GLint uniform = glGetUniformLocation(ref_, uniformName);
    if(uniform == -1){
        throw std::runtime_error(std::string("ShaderProgram uniform not found: ") + uniformName);
    }
    return uniform;
}

#define ATTRIB_N_UNIFORM_SETTERS(OGL_TYPE, TYPE_PREFIX, TYPE_SUFFIX) \
\
void ShaderProgram::setAttrib(const GLchar* name, OGL_TYPE v0) \
{ assert(isInUse()); glVertexAttrib ## TYPE_PREFIX ## 1 ## TYPE_SUFFIX (attrib(name), v0); } \
void ShaderProgram::setAttrib(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1) \
{ assert(isInUse()); glVertexAttrib ## TYPE_PREFIX ## 2 ## TYPE_SUFFIX (attrib(name), v0, v1); } \
void ShaderProgram::setAttrib(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2) \
{ assert(isInUse()); glVertexAttrib ## TYPE_PREFIX ## 3 ## TYPE_SUFFIX (attrib(name), v0, v1, v2); } \
void ShaderProgram::setAttrib(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3) \
{ assert(isInUse()); glVertexAttrib ## TYPE_PREFIX ## 4 ## TYPE_SUFFIX (attrib(name), v0, v1, v2, v3); } \
\
void ShaderProgram::setAttrib1v(const GLchar* name, const OGL_TYPE* v) \
{ assert(isInUse()); glVertexAttrib ## TYPE_PREFIX ## 1 ## TYPE_SUFFIX ## v (attrib(name), v); } \
void ShaderProgram::setAttrib2v(const GLchar* name, const OGL_TYPE* v) \
{ assert(isInUse()); glVertexAttrib ## TYPE_PREFIX ## 2 ## TYPE_SUFFIX ## v (attrib(name), v); } \
void ShaderProgram::setAttrib3v(const GLchar* name, const OGL_TYPE* v) \
{ assert(isInUse()); glVertexAttrib ## TYPE_PREFIX ## 3 ## TYPE_SUFFIX ## v (attrib(name), v); } \
void ShaderProgram::setAttrib4v(const GLchar* name, const OGL_TYPE* v) \
{ assert(isInUse()); glVertexAttrib ## TYPE_PREFIX ## 4 ## TYPE_SUFFIX ## v (attrib(name), v); } \
\
void ShaderProgram::setUniform(const GLchar* name, OGL_TYPE v0) \
{ assert(isInUse()); glUniform1 ## TYPE_SUFFIX (uniform(name), v0); } \
void ShaderProgram::setUniform(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1) \
{ assert(isInUse()); glUniform2 ## TYPE_SUFFIX (uniform(name), v0, v1); } \
void ShaderProgram::setUniform(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2) \
{ assert(isInUse()); glUniform3 ## TYPE_SUFFIX (uniform(name), v0, v1, v2); } \
void ShaderProgram::setUniform(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3) \
{ assert(isInUse()); glUniform4 ## TYPE_SUFFIX (uniform(name), v0, v1, v2, v3); } \
\
void ShaderProgram::setUniform1v(const GLchar* name, const OGL_TYPE* v, GLsizei count) \
{ assert(isInUse()); glUniform1 ## TYPE_SUFFIX ## v (uniform(name), count, v); } \
void ShaderProgram::setUniform2v(const GLchar* name, const OGL_TYPE* v, GLsizei count) \
{ assert(isInUse()); glUniform2 ## TYPE_SUFFIX ## v (uniform(name), count, v); } \
void ShaderProgram::setUniform3v(const GLchar* name, const OGL_TYPE* v, GLsizei count) \
{ assert(isInUse()); glUniform3 ## TYPE_SUFFIX ## v (uniform(name), count, v); } \
void ShaderProgram::setUniform4v(const GLchar* name, const OGL_TYPE* v, GLsizei count) \
{ assert(isInUse()); glUniform4 ## TYPE_SUFFIX ## v (uniform(name), count, v); }

ATTRIB_N_UNIFORM_SETTERS(GLfloat, , f);
ATTRIB_N_UNIFORM_SETTERS(GLdouble, , d);
ATTRIB_N_UNIFORM_SETTERS(GLint, I, i);
ATTRIB_N_UNIFORM_SETTERS(GLuint, I, ui);

void ShaderProgram::setUniformMatrix2(const GLchar* name, const GLfloat* v, GLsizei count, GLboolean transpose) {
    assert(isInUse());
    glUniformMatrix2fv(uniform(name), count, transpose, v);
}

void ShaderProgram::setUniformMatrix3(const GLchar* name, const GLfloat* v, GLsizei count, GLboolean transpose) {
    assert(isInUse());
    glUniformMatrix3fv(uniform(name), count, transpose, v);
}

void ShaderProgram::setUniformMatrix4(const GLchar* name, const GLfloat* v, GLsizei count, GLboolean transpose) {
    assert(isInUse());
    glUniformMatrix4fv(uniform(name), count, transpose, v);
}

void ShaderProgram::setUniform(const GLchar* name, const glm::mat2& m, GLboolean transpose) {
    assert(isInUse());
    glUniformMatrix2fv(uniform(name), 1, transpose, glm::value_ptr(m));
}

void ShaderProgram::setUniform(const GLchar* name, const glm::mat3& m, GLboolean transpose) {
    assert(isInUse());
    glUniformMatrix3fv(uniform(name), 1, transpose, glm::value_ptr(m));
}

void ShaderProgram::setUniform(const GLchar* name, const glm::mat4& m, GLboolean transpose) {
    assert(isInUse());
    glUniformMatrix4fv(uniform(name), 1, transpose, glm::value_ptr(m));
}

void ShaderProgram::setUniform(const GLchar* uniformName, const glm::vec2& v) {
    setUniform2v(uniformName, glm::value_ptr(v));
}

void ShaderProgram::setUniform(const GLchar* uniformName, const glm::vec3& v) {
    setUniform3v(uniformName, glm::value_ptr(v));
}

void ShaderProgram::setUniform(const GLchar* uniformName, const glm::vec4& v) {
    setUniform4v(uniformName, glm::value_ptr(v));
}
