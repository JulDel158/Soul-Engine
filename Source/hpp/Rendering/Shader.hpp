#ifndef SHADER_H
#define SHADER_H

#include <string>

#include "glad/gl.h"
#include "glm/glm.hpp"

class Shader
{
private:
    // ID of program where shaders will be linked
    GLuint id_;
    bool is_valid_;

public:
    // When copying a shader we just want the id and validity
    Shader();
    // note: geometry source code is optional
    Shader(const char *vertexSource, const char *fragmentSource, const char *geometrySource = nullptr);
    ~Shader()=default; // Resource Manager will be in charge of calling clear as destructor calls after going out of scope could lead to accidental clearing
    
    Shader(const Shader& shader);
    Shader& operator=(const Shader& shader);
    
    Shader& Use();
    void Clear();
    unsigned int GetId() const;
    bool IsValid() const;
    
    // functions for setting uniform on the shaders
    void SetUniformFloat    (const char *name, const float value, const bool useShader = false);
    void SetUniformInteger  (const char *name, const int value, const bool useShader = false);
    void SetUniformVector2f (const char *name, const float x, const float y, const bool useShader = false);
    void SetUniformVector2f (const char *name, const glm::vec2 &value, const bool useShader = false);
    void SetUniformVector3f (const char *name, const float x, const float y, const float z, const bool useShader = false);
    void SetUniformVector3f (const char *name, const glm::vec3 &value, const bool useShader = false);
    void SetUniformVector4f (const char *name, const float x, const float y, const float z, const float w, const bool useShader = false);
    void SetUniformVector4f (const char *name, const glm::vec4 &value, const bool useShader = false);
    void SetUniformMatrix4  (const char *name, const glm::mat4 &matrix, const bool useShader = false);
    
private:
    void Compile(const char *vertexSource, const char *fragmentSource, const char *geometrySource = nullptr);
    // checks if compilation or linking failed and if so, print the error logs
    void CheckCompileErrors(const GLuint object, const std::string& type); 
};

#endif