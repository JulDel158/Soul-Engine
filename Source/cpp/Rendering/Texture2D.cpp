#include "Rendering/Texture2D.hpp"

Texture2D::Texture2D():
id_(-1),
width_(0), 
height_(0), 
internal_format_(0), 
image_format_(0), 
wrap_s_(0), 
wrap_t_(0), 
filter_min_(0), 
filter_max_(0),
is_valid_(false)
{
}

Texture2D::Texture2D(GLsizei width, GLsizei height, GLint internalFormat, GLenum imageFormat, GLint wrapS,
                     GLint wrapT, GLint filterMin, GLint filterMax, const unsigned char* data):
width_(width), 
height_(height), 
internal_format_(internalFormat), 
image_format_(imageFormat), 
wrap_s_(wrapS), 
wrap_t_(wrapT), 
filter_min_(filterMin), 
filter_max_(filterMax),
is_valid_(true)
{
    glGenTextures(1, &id_);
    Generate(data);
}

void Texture2D::Bind() const
{
    if (is_valid_)
    {
        glBindTexture(GL_TEXTURE_2D, id_);
    }
}

void Texture2D::Clear()
{
    if (is_valid_)
    {
        glDeleteTextures(1, &id_);
        is_valid_ = false;
    }
}

bool Texture2D::IsValid() const
{
    return is_valid_;
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
