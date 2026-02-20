#include "Rendering/TextRenderer.hpp"

#include <iostream>
#include <unordered_map>

#include "glad/gl.h"
#include "glm/ext/matrix_clip_space.hpp"

#include "ResourceManagement/ResourceManager.hpp"


TextRenderer::TextRenderer()
{
    InitRenderData();
}

TextRenderer::~TextRenderer()
{
    // Since we move data to this object we are safe to clear it
}

void TextRenderer::RenderText(const std::string& text, float x, float y, const float scale, const glm::vec3 color, const std::string& font)
{
    // get font
    auto& resourceManager = ResourceManager::Instance();
    if (!resourceManager.ContainsFont(font))
    {
        std::cout << "WARNING::RenderText: Font: " << font << " not found! " << std::endl;
        return;
    }
    
    auto& fontMap = resourceManager.GetFont(font);
    
    // activate corresponding render state	
    shader_.Use();
    shader_.SetUniformVector3f(TEXT_COLOR_UNIFORM.data(), color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(vao_);

    // iterate through all characters
    for (char character : text)
    {
        TextCharacter textCharacter = fontMap[character];

        float xPos = x + static_cast<float>(textCharacter.bearing_.x) * scale;
        float yPos = y + static_cast<float>(fontMap['H'].bearing_.y - textCharacter.bearing_.y) * scale;

        float w = static_cast<float>(textCharacter.size_.x) * scale;
        float h = static_cast<float>(textCharacter.size_.y) * scale;
        
        // update VBO for each character
        float vertices[6][4] = {
            { xPos,     yPos + h,   0.0f, 1.0f },
            { xPos + w, yPos,       1.0f, 0.0f },
            { xPos,     yPos,       0.0f, 0.0f },

            { xPos,     yPos + h,   0.0f, 1.0f },
            { xPos + w, yPos + h,   1.0f, 1.0f },
            { xPos + w, yPos,       1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, textCharacter.id_);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph
        x += static_cast<float>(textCharacter.advance_ >> 6) * scale; // bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void TextRenderer::SwapShader(const std::string& name)
{
    auto& resourceManager = ResourceManager::Instance();
    if (!resourceManager.ContainsShader(name))
    {
        return;
    }
    
    const Settings settings = resourceManager.GetSettings();
    
    // load and configure shader
    shader_ = resourceManager.GetShader(name);
    shader_.SetUniformMatrix4(PROJECTION_UNIFORM.data(), 
        glm::ortho(0.0f, static_cast<float>(settings.screen_width_), static_cast<float>(settings.screen_height_), 0.0f), 
        true);
    shader_.SetUniformInteger(TEXT_UNIFORM.data(), 0);
}

void TextRenderer::SwapShader(const Shader& shader)
{
    const Settings settings = ResourceManager::Instance().GetSettings();
    
    // load and configure shader
    shader_ = shader;
    shader_.SetUniformMatrix4(PROJECTION_UNIFORM.data(), 
        glm::ortho(0.0f, static_cast<float>(settings.screen_width_), static_cast<float>(settings.screen_height_), 0.0f), 
        true);
    shader_.SetUniformInteger(TEXT_UNIFORM.data(), 0);
}

void TextRenderer::InitRenderData()
{
    // configure VAO/VBO for texture quads
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
