#ifndef TEXT_RENDERER_HPP
#define TEXT_RENDERER_HPP
#pragma once

#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float2.hpp"

#include "Shader.hpp"
#include "StringGlobals.hpp"

#include <string>

class TextRenderer
{
private:
    Shader shader_;
    unsigned int vbo_;
    unsigned int vao_;
    
public:
    TextRenderer();
    ~TextRenderer();
    
    void RenderText(const std::string& text, glm::vec2 position, const float scale, const glm::vec3 color = glm::vec3(1.0f), const std::string& font = FONT_ARIAL.data()) const;
    
    void SwapShader(const std::string& name);
    void SwapShader(const Shader& shader);
    
private:
    void InitRenderData();
};

#endif