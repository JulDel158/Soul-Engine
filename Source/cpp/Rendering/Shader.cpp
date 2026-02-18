#include "Rendering/Shader.hpp"

#include <iostream>

#include "glm/gtc/type_ptr.hpp"

Shader::Shader():
id_(-1),
is_valid_(false)
{
}

Shader::Shader(const char* vertexSource, const char* fragmentSource, const char* geometrySource)
{
    Compile(vertexSource, fragmentSource, geometrySource);
}

Shader::Shader(const Shader& shader)
{
    id_ = shader.GetId();
    is_valid_ = shader.IsValid();
}

Shader& Shader::operator=(const Shader& shader)
{
    if (this == &shader)
    {
        return *this;
    }
    
    id_ = shader.GetId();
    is_valid_ = shader.IsValid();
    return *this;
}

void Shader::Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource)
{
    is_valid_ = true;
    GLuint geometryId = 0;
    
    // We will load each shader if possible.
    // Note that a vertex shader and a fragment shader are ALWAYS required for a program
    
    // Vertex Shader
    GLuint vertexId = glCreateShader(GL_VERTEX_SHADER);
    // ReSharper disable once CppZeroConstantCanBeReplacedWithNullptr
    glShaderSource(vertexId, 1, &vertexSource, nullptr);
    glCompileShader(vertexId);
    CheckCompileErrors(vertexId, "VERTEX");
    
    // Fragment Shader
    GLuint fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
    // ReSharper disable once CppZeroConstantCanBeReplacedWithNullptr
    glShaderSource(fragmentId, 1, &fragmentSource, nullptr); 
    glCompileShader(fragmentId);
    CheckCompileErrors(fragmentId, "FRAGMENT");
    
    // Geometry Shader
    if (geometrySource != nullptr)
    {
        geometryId = glCreateShader(GL_GEOMETRY_SHADER);
        // ReSharper disable once CppZeroConstantCanBeReplacedWithNullptr
        glShaderSource(geometryId, 1, &geometrySource, nullptr);
        glCompileShader(geometryId);
        CheckCompileErrors(geometryId, "GEOMETRY");
    }
    
    // Now we must create the program and link the shaders
    id_ = glCreateProgram();
    glAttachShader(id_, vertexId);
    glAttachShader(id_, fragmentId);
    if (geometrySource != nullptr)
    {
        glAttachShader(id_, geometryId);
    }
    
    glLinkProgram(id_);
    CheckCompileErrors(id_, "PROGRAM");
    
    // after linking we can detach and delete shaders
    glDetachShader(id_, vertexId);
    glDetachShader(id_, fragmentId);
    if (geometrySource != nullptr)
    {
        glDetachShader(id_, geometryId);
    }
    
    glDeleteShader(vertexId);
    glDeleteShader(fragmentId);
    if (geometrySource != nullptr)
    {
        glDeleteShader(geometryId);
    }
}

Shader& Shader::Use()
{
    if (is_valid_)
    {
        glUseProgram(id_);
    }
    return *this;
}
void Shader::Clear()
{
    if (is_valid_)
    {
        glDeleteProgram(id_);
        id_ = -1;
        is_valid_ = false;
    }
}

unsigned int Shader::GetId() const
{
    return id_;
}

bool Shader::IsValid() const
{
    return is_valid_;
}

void Shader::SetUniformFloat(const char* name, const float value, const bool useShader)
{
    if (useShader == true)
    {
        Use();
    }
    glUniform1f(glGetUniformLocation(id_, name), value);
}

void Shader::SetUniformInteger(const char* name, const int value, const bool useShader)
{
    if (useShader == true)
    {
        Use();
    }
    glUniform1i(glGetUniformLocation(id_, name), value);
}

void Shader::SetUniformVector2f(const char* name, const float x, const float y, const bool useShader)
{
    if (useShader == true)
    {
        Use();
    }
    glUniform2f(glGetUniformLocation(id_, name), x, y);
}

void Shader::SetUniformVector2f(const char* name, const glm::vec2& value, const bool useShader)
{
    if (useShader == true)
    {
        Use();
    }
    glUniform2f(glGetUniformLocation(id_, name), value.x, value.y);
}

void Shader::SetUniformVector3f(const char* name, const float x, const float y, const float z, const bool useShader)
{
    if (useShader == true)
    {
        Use();
    }
    glUniform3f(glGetUniformLocation(id_, name), x, y, z);
}

void Shader::SetUniformVector3f(const char* name, const glm::vec3& value, bool useShader)
{
    if (useShader == true)
    {
        Use();
    }
    glUniform3f(glGetUniformLocation(id_, name), value.x, value.y, value.z);
}

void Shader::SetUniformVector4f(const char* name, const float x, const float y, const float z, const float w, const bool useShader)
{
    if (useShader == true)
    {
        Use();
    }
    glUniform4f(glGetUniformLocation(id_, name), x, y, z, w);
}

void Shader::SetUniformVector4f(const char* name, const glm::vec4& value, const bool useShader)
{
    if (useShader == true)
    {
        Use();
    }
    glUniform4f(glGetUniformLocation(id_, name), value.x, value.y, value.z, value.w);
}

void Shader::SetUniformMatrix4(const char* name, const glm::mat4& matrix, const bool useShader)
{
    if (useShader == true)
    {
        Use();
    }
    glUniformMatrix4fv(glGetUniformLocation(id_, name), 1, false, glm::value_ptr(matrix));
}

void Shader::CheckCompileErrors(const GLuint object, const std::string& type)
{
    GLint success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
            is_valid_ = false;
        }
    }
    else
    {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
            is_valid_ = false;
        }
    }
}
