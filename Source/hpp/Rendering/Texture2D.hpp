#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H

#include "glad/gl.h"

class Texture2D
{
private:
    GLuint id_;

    GLsizei width_, height_; // width and height of loaded image in pixels
    // texture Format
    GLint internal_format_; // format of texture object
    GLenum image_format_; // format of loaded image
    // texture configuration
    GLint wrap_s_; // wrapping mode on S axis
    GLint wrap_t_; // wrapping mode on T axis
    GLint filter_min_; // filtering mode if texture pixels < screen pixels
    GLint filter_max_; // filtering mode if texture pixels > screen pixels
    
    bool is_valid_;
    
public:
    Texture2D();
    Texture2D(GLsizei width, GLsizei height, GLint internalFormat, GLenum imageFormat, 
        GLint wrapS, GLint wrapT, GLint filterMin, GLint filterMax, const unsigned char* data);
    ~Texture2D()=default; // Resource Manager will be in charge of calling clear as destructor calls after going out of scope could lead to accidental clearing
    
    // binds the texture as the current active GL_TEXTURE_2D texture object
    void Bind() const;
    
    // calls glDeleteTextures and passes our id
    void Clear();
    
    bool IsValid() const;
    
private:
    // generates texture from image data
    void Generate(const unsigned char* data) const;
};

#endif