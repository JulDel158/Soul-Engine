#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "glad/gl.h"

class Texture2D
{
private:
    GLuint id_;

    GLsizei width_, height_; // width and height of loaded image in pixels
    // texture Format
    GLint internal_format_; // format of texture object
    unsigned int image_format_; // format of loaded image
    // texture configuration
    GLint wrap_s_; // wrapping mode on S axis
    GLint wrap_t_; // wrapping mode on T axis
    GLint filter_min_; // filtering mode if texture pixels < screen pixels
    GLint filter_max_; // filtering mode if texture pixels > screen pixels
    
public:
    Texture2D()=delete;
    Texture2D(const GLsizei width, const GLsizei height, const unsigned char* data);
    ~Texture2D();
    
    // binds the texture as the current active GL_TEXTURE_2D texture object
    void Bind() const;
    
private:
    // generates texture from image data
    void Generate(const unsigned char* data) const;
};

#endif