#include "Rendering/SpriteRenderer.hpp"

#include "glad/gl.h"
#include "glm/gtc/matrix_transform.hpp"

#include "ResourceManagement/ResourceManager.hpp"

SpriteRenderer::SpriteRenderer(ESpriteCentering centering) :
centering_(centering)
{
    InitRenderData();
}

SpriteRenderer::SpriteRenderer(const Shader& shader, ESpriteCentering centering) :
centering_(centering)
{
    shader_ = shader;
    centering_ = centering;
    InitRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1, &vao_);
    // Resource manager is in charge of clearing shaders
}

void SpriteRenderer::DrawSprite(const Texture2D& texture, const glm::vec2& position, const glm::vec2& size, 
    const float rotation, const glm::vec3& color) const
{
    shader_.Use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));
    
    shader_.SetUniformMatrix4("model", model);
    shader_.SetUniformVector3f("spriteColor", color);
    
    glActiveTexture(GL_TEXTURE0);
    texture.Bind();
    
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

bool SpriteRenderer::SwapShader(const std::string& name)
{
    ResourceManager& instance = ResourceManager::Instance();
    const bool result = instance.ContainsShader(name);
    if (result)
    {
        shader_ = instance.GetShader(name);
    }
    return result;
}

void SpriteRenderer::SwapShader(const Shader& shader)
{
    shader_ = shader;
}

void SpriteRenderer::InitRenderData()
{
    // configure VAO/VBO
    unsigned int vbo;
    glm::vec2 topLeft, bottomLeft, bottomRight, topRight;
    switch (centering_)
    {
        case ESpriteCentering::Top_Left:
        topLeft = glm::vec2(0.0f, 0.0f);
        bottomLeft = glm::vec2(0.0f, 1.0f);
        topRight = glm::vec2(1.0f, 0.0f);
        bottomRight = glm::vec2(1.0f, 1.0f);
        break;
        case ESpriteCentering::Top_Right:
        topLeft = glm::vec2(-1.0f, 0.0f);
        bottomLeft = glm::vec2(-1.0f, 1.0f);
        topRight = glm::vec2(0.0f, 0.0f);
        bottomRight = glm::vec2(0.0f, 1.0f);
        break;
        case ESpriteCentering::Bottom_Left:
        topLeft = glm::vec2(0.0f, -1.0f);
        bottomLeft = glm::vec2(0.0f, 0.0f);
        topRight = glm::vec2(1.0f, -1.0f);
        bottomRight = glm::vec2(1.0f, 0.0f);
        break;
        case ESpriteCentering::Bottom_Right:
        topLeft = glm::vec2(-1.0f, -1.0f);
        bottomLeft = glm::vec2(-1.0f, 0.0f);
        topRight = glm::vec2(0.0f, -1.0f);
        bottomRight = glm::vec2(0.0f, 0.0f);
        break;
    case ESpriteCentering::Center:
        topLeft = glm::vec2(-0.5f, -0.5f);
        bottomLeft = glm::vec2(-0.5f, 0.5f);
        topRight = glm::vec2(0.5f, -0.5f);
        bottomRight = glm::vec2(0.5f, 0.5f);
        break;
    }
    
    float vertices[] = { 
        // pos      // tex
        bottomLeft.x, bottomLeft.y, 0.0f, 1.0f,
        topRight.x, topRight.y, 1.0f, 0.0f,
        topLeft.x, topLeft.y, 0.0f, 0.0f, 
        bottomLeft.x, bottomLeft.y, 0.0f, 1.0f,
        bottomRight.x, bottomRight.y, 1.0f, 1.0f,
        topRight.x, topRight.y, 1.0f, 0.0f
    };
    
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindVertexArray(vao_);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), static_cast<void*>(0)); //NOLINT
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}