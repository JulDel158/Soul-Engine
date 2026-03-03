#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <string>

#include "glm/vec3.hpp"

#include "Shader.hpp"
#include "StringGlobals.hpp"

class TextRenderer
{
private:
    Shader shader_;
    unsigned int vbo_;
    unsigned int vao_;
    
public:
    TextRenderer();
    ~TextRenderer() = default;
    
    void RenderText(const std::string& text, float x, float y, const float scale, const glm::vec3 color = glm::vec3(1.0f), const std::string& font = FONT_ARIAL.data()) const;
    
    void SwapShader(const std::string& name);
    void SwapShader(const Shader& shader);
    
private:
    void InitRenderData();
};
#endif
