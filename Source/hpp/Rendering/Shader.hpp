#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <optional>

#include "glm/glm.hpp"

class Shader
{
private:
    // ID of program where shaders will be linked
    std::optional<unsigned int> id_;

public:
    // When copying a shader we just want the id and validity
    Shader();
    Shader(const char *vertexSource, const char *fragmentSource, const char *geometrySource = nullptr); // note: geometry source code is optional
    ~Shader()=default; // Resource Manager will be in charge of calling clear as destructor calls after going out of scope could lead to accidental clearing
    
    Shader(const Shader& shader);
    Shader(Shader&& shader) noexcept;
    Shader& operator=(const Shader& shader);
    Shader& operator=(Shader&& shader) noexcept;
    
    void            Use() const;
    void            Clear();
    unsigned int    GetId() const;
    bool            IsValid() const;
    
    // functions for setting uniform on the shaders
    void SetUniformFloat    (const char *name, const float value, const bool useProgram = false) const;
    void SetUniformInteger  (const char *name, const int value, const bool useProgram = false) const;
    void SetUniformVector2f (const char *name, const float x, const float y, const bool useProgram = false)const;
    void SetUniformVector2f (const char *name, const glm::vec2 &value, const bool useProgram = false) const;
    void SetUniformVector3f (const char *name, const float x, const float y, const float z, const bool useProgram = false) const;
    void SetUniformVector3f (const char *name, const glm::vec3 &value, const bool useProgram = false) const;
    void SetUniformVector4f (const char *name, const float x, const float y, const float z, const float w, const bool useProgram = false) const;
    void SetUniformVector4f (const char *name, const glm::vec4 &value, const bool useProgram = false) const;
    void SetUniformMatrix4  (const char *name, const glm::mat4 &matrix, const bool useProgram = false) const;
    
private:
    
    void Compile(const char *vertexSource, const char *fragmentSource, const char *geometrySource = nullptr);
    
    // checks if shader compilation or program linking failed
    bool CheckCompileErrors(const unsigned int object, const std::string& type); 
};

#endif