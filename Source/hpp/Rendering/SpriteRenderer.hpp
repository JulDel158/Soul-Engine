#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include "Shader.hpp"
#include "Texture2D.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "EngineDataStructures.hpp"

#include <string>

class SpriteRenderer
{
private:
    Shader shader_;
    unsigned int vao_;
    unsigned int vbo_;
    ESpriteCentering centering_;
    
public:
    SpriteRenderer()=delete;
    SpriteRenderer(const SpriteRenderer&) = delete;
    SpriteRenderer(SpriteRenderer&&) = delete;
    SpriteRenderer& operator=(const SpriteRenderer&) = delete;
    SpriteRenderer& operator=(SpriteRenderer&&) = delete;
    explicit SpriteRenderer(const ESpriteCentering centering = ESpriteCentering::Center);
    explicit SpriteRenderer(const Shader& shader, const ESpriteCentering centering = ESpriteCentering::Center);
    ~SpriteRenderer();
    
    void DrawSprite(const Texture2D& texture, const glm::vec2& position, const glm::vec2& size,
        const float rotation = 0.0f, const glm::vec3& color = glm::vec3(1.0f)) const;
    
    bool SwapShader(const std::string& name);
    void SwapShader(const Shader& shader);
    
private:
    
    void InitRenderData();
};


#endif
