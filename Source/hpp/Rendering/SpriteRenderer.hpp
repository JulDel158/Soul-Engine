#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include "Shader.hpp"
#include "Texture2D.hpp"
#include "glm/glm.hpp"
#include "EngineDataStructures.hpp"

class SpriteRenderer
{
public:
    
    SpriteRenderer(const Shader & shader, ESpriteCentering centering = ESpriteCentering::CENTER);
    ~SpriteRenderer();
    
    void DrawSprite(const Texture2D& texture, const glm::vec2 position, const glm::vec2 size,
        const float rotation = 0.0f, const glm::vec3 color = glm::vec3(1.0f));
    
private:
    
    GLuint vao_;
    ESpriteCentering centering_;
    Shader shader_;
    
    SpriteRenderer()=delete;
    
    void InitRenderData();
};


#endif
