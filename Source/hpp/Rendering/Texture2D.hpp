#pragma once

class Texture2D
{
private:
    unsigned int id_;
	// width and height of loaded image in pixels
    int width_;
	int height_; 
    // texture Format
    int internal_format_; // format of texture object
    unsigned int image_format_; // format of loaded image
    // texture configuration
    int wrap_s_; // wrapping mode on S axis
    int wrap_t_; // wrapping mode on T axis
    int filter_min_; // filtering mode if texture pixels < screen pixels
    int filter_max_; // filtering mode if texture pixels > screen pixels
    bool is_valid_;
    
public:
    Texture2D();
    Texture2D(const int width, const int height, const int internalFormat, const unsigned int imageFormat, 
        const int wrapS, const int wrapT, const int filterMin, const int filterMax, const unsigned char* data);
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