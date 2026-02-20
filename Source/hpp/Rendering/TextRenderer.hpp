#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <unordered_map>
#include <string>

#include "glm/vec3.hpp"

#include "EngineDataStructures.hpp"
#include "Shader.hpp"

class TextRenderer
{
private:
    std::unordered_map<char, TextCharacter> font_;
    std::string font_key_;
    Shader shader_;
    unsigned int vbo_;
    unsigned int vao_;
    
public:
    TextRenderer();
    ~TextRenderer();
    
    void RenderText(const std::string& text, float x, float y, const float scale, const glm::vec3 color = glm::vec3(1.0f));
    
    void SwapFont(const std::string& name);
    void SwapShader(const std::string& name);
    void SwapShader(const Shader& shader);
    
private:
    void InitRenderData();
};
#endif
