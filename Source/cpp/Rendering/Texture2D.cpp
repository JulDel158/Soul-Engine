#include "Rendering/Texture2D.hpp"

Texture2D::Texture2D(const GLsizei width, const GLsizei height, const unsigned char* data) :
width_(width), 
height_(height), 
internal_format_(GL_RGB), 
image_format_(GL_RGB), 
wrap_s_(GL_REPEAT), 
wrap_t_(GL_REPEAT), 
filter_min_(GL_LINEAR), 
filter_max_(GL_LINEAR),
cleared_(false)
{
    glGenTextures(1, &id_);
    Generate(data);
}

Texture2D::~Texture2D()
{
    Clear();
}

void Texture2D::Bind() const
{
    if (!cleared_)
    {
        glBindTexture(GL_TEXTURE_2D, id_);
    }
}

void Texture2D::Clear()
{
    if (!cleared_)
    {
        glDeleteTextures(1, &id_);
        cleared_ = true;
    }
}

void Texture2D::Generate(const unsigned char* data) const
{
    // create Texture
    glBindTexture(GL_TEXTURE_2D, id_);
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format_, width_, height_, 0, image_format_, GL_UNSIGNED_BYTE, data);
    // set Texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_max_);
    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}
