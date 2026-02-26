#include "ResourceManagement/ResourceManager.hpp"

#include "glad/gl.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "rapidjson/document.h"

#include <iostream>
#include <filesystem>
#include <sstream>
#include <fstream>
#include <utility>

#include "StringGlobals.hpp"

ResourceManager& ResourceManager::Instance()
{
    static ResourceManager instance = ResourceManager();
    return instance;
}

ResourceManager::ResourceManager()
{
    shaders_ = std::unordered_map<std::string, Shader>();
    textures_ = std::unordered_map<std::string, Texture2D>();
    settings_ = Settings();
    
    OpenFreeTypeLibrary();
}

ResourceManager::~ResourceManager()
{
    Clear();
}

Shader ResourceManager::LoadShader(const char* vertexShaderFile, const char* fragmentShaderFile,
const char* geometryShaderFile, const std::string& name)
{
    if (shaders_.contains(name))
    {
        std::cout << "Warning: Overriding existing shader: " << name << std::endl;   // NOLINT(performance-avoid-endl)
        shaders_[name].Clear();
        shaders_.erase(name);
    }
    
    std::filesystem::path vertexPath = std::filesystem::path(vertexShaderFile);
    std::filesystem::path fragmentPath = std::filesystem::path(fragmentShaderFile);
    std::filesystem::path geometryPath = geometryShaderFile != nullptr ? std::filesystem::path(geometryShaderFile) : "";
    
    shaders_[name] = LoadShaderFromFile(vertexPath, fragmentPath, geometryPath, geometryShaderFile != nullptr);
    return shaders_[name];
}

Shader ResourceManager::GetShader(const std::string& name)
{
    return shaders_[name];
}

bool ResourceManager::ContainsShader(const std::string& name) const
{
    return shaders_.contains(name);
}

Texture2D ResourceManager::LoadTexture2D(const char* filePath, bool alpha, const std::string& name)
{
    if (textures_.contains(name))
    {
        std::cout << "Warning: Overriding existing texture: " << name << std::endl;
        textures_[name].Clear();
        textures_.erase(name);
    }
    
    textures_[name] = LoadTextureFromFile(filePath, alpha);
    return textures_[name];
}

Texture2D ResourceManager::GetTexture2D(const std::string& name)
{
    return textures_[name];
}

void ResourceManager::LoadFont(const char* filePath, const unsigned int fontSize, const std::string& name)
{
    if (!is_ft_open_)
    {
        std::cout << "WARNING::LoadFont: FT Library was not open  " << std::endl;
        return;
    }
    
    // load font as face
    FT_Face face;
    if (FT_New_Face(free_type_library_, filePath, 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }
    
    // set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, fontSize);
    // disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
    // then for the first 128 ASCII characters, preload/compile their characters and store them
    for (GLubyte i = 0; i < 128; ++i) 
    {
        // load character glyph 
        if (FT_Load_Char(face, i, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            static_cast<GLint>(face->glyph->bitmap.width),
            static_cast<GLint>(face->glyph->bitmap.rows),
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
            );
        
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
       
        // now store character for later use
        auto character = TextCharacter(
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        );
        fonts_[name].insert(std::pair<char, TextCharacter>(i, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // clear generated face once done, not closing FT until we are done loading fonts into memory
    FT_Done_Face(face);
}

std::unordered_map<char, TextCharacter>& ResourceManager::GetFont(const std::string& name)
{
    return fonts_[name];
}

void ResourceManager::LoadSettings(Settings& settings)
{
    std::filesystem::path settingsPath = std::filesystem::path(GRAPHIC_SETTINGS.data());
    try
    {
        std::ifstream settingsFile(settingsPath);
        std::string fileData((std::istreambuf_iterator<char>(settingsFile)),std::istreambuf_iterator<char>());
        settingsFile.close();
        
        rapidjson::Document settingsJson;
        settingsJson.Parse(fileData.c_str());
        
        if (settingsJson.HasParseError())
        {
            std::cout << "ERROR:Failed to parse settings: " << settingsJson.GetParseError() << std::endl;
        }
        
        // graphics
        if (settingsJson.HasMember(SCREEN_WIDTH.data())) // NOLINT
        {
            settings_.screen_width_ = settingsJson[SCREEN_WIDTH.data()].GetInt();
        }
        
        if (settingsJson.HasMember(SCREEN_HEIGHT.data())) // NOLINT
        {
            settings_.screen_height_ = settingsJson[SCREEN_HEIGHT.data()].GetInt();
        }
        
        if (settingsJson.HasMember(VSYNC.data())) // NOLINT
        {
            settings_.vsync_ = settingsJson[VSYNC.data()].GetBool();
        }
        
        // Sound
        if (settingsJson.HasMember(GENERAL_VOLUME.data())) // NOLINT
        {
            settings_.general_volume_ = settingsJson[GENERAL_VOLUME.data()].GetFloat();
        }
        
        if (settingsJson.HasMember(MUSIC_VOLUME.data())) // NOLINT
        {
            settings_.music_volume_ = settingsJson[MUSIC_VOLUME.data()].GetFloat();
        }
        
        if (settingsJson.HasMember(EFFECTS_VOLUME.data())) // NOLINT
        {
            settings_.effects_volume_ = settingsJson[EFFECTS_VOLUME.data()].GetFloat();
        }
        
        if (settingsJson.HasMember(DIALOGUE_VOLUME.data())) // NOLINT
        {
            settings_.dialogue_volume_ = settingsJson[DIALOGUE_VOLUME.data()].GetFloat();
        }
        
    }
    catch (std::exception& e)
    {
        std::cout << "ERROR::Settings: Failed to read Settings file" << e.what() << std::endl;
    }
    
    settings = Settings(settings_);
}

Settings ResourceManager::GetSettings() const
{
    return settings_;
}

void ResourceManager::ReclaimFontMemory(InnerMap& font, const std::string& name)
{
    fonts_[name] = std::move(font);
}

bool ResourceManager::ContainsTexture2D(const std::string& name) const
{
    return textures_.contains(name);
}

bool ResourceManager::ContainsFont(const std::string& name) const
{
    return fonts_.contains(name);
}

void ResourceManager::Clear()
{
    // release these resources from openGL
    for (auto& pair : shaders_) // NOLINT
    {
        pair.second.Clear();
    }
    
    for (auto& pair : textures_) // NOLINT
    {
        pair.second.Clear();
    }
    
    for (auto& pair : fonts_) // NOLINT
    {
        pair.second.clear();
    }
    
    shaders_.clear();
    textures_.clear();
    fonts_.clear();
    
    CloseFreeTypeLibrary();
}

void ResourceManager::OpenFreeTypeLibrary()
{
    if (is_ft_open_)
    {
        return;
    }
    
    if (const auto error = FT_Init_FreeType(&free_type_library_); error)
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return;
    }
    is_ft_open_ = true;
}

void ResourceManager::CloseFreeTypeLibrary()
{
    if (!is_ft_open_)
    {
        return;
    }
    
    if (const auto error = FT_Done_FreeType(free_type_library_); error)
    {
        std::cout << "ERROR::FREETYPE: Could not close FreeType Library" << std::endl;
        return;
    }
    is_ft_open_ = false;
}

Shader ResourceManager::LoadShaderFromFile(const std::filesystem::path& vertexShaderPath, 
    const std::filesystem::path& fragmentShaderPath,
    const std::filesystem::path& geometryShaderPath, bool hasGeometryShader)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try
    {
        // open files
        std::ifstream vertexShaderFile(vertexShaderPath);
        std::ifstream fragmentShaderFile(fragmentShaderPath);
        std::stringstream vertexShaderStream, fragmentShaderStream;
        // read file's buffer contents into streams
        vertexShaderStream << vertexShaderFile.rdbuf();
        fragmentShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // convert stream into string
        vertexCode = vertexShaderStream.str();
        fragmentCode = fragmentShaderStream.str();
        // if geometry shader path is present, also load a geometry shader
        if (hasGeometryShader)
        {
            std::ifstream geometryShaderFile(geometryShaderPath);
            std::stringstream geometryShaderStream;
            geometryShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = geometryShaderStream.str();
        }
    }
    catch (std::exception& e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << e.what() << std::endl;
    }
    const char *vertexShaderCode = vertexCode.c_str();
    const char *fragmentShaderCode = fragmentCode.c_str();
    const char *geometryShaderCode = geometryCode.c_str();
    // 2. now create shader object from source code
    Shader shader = Shader(vertexShaderCode, fragmentShaderCode, hasGeometryShader ? geometryShaderCode : nullptr);
    return shader;
}

Texture2D ResourceManager::LoadTextureFromFile(const char* file, bool alpha)
{
    GLint internalFormat; // format of texture object
    GLenum imageFormat;
    GLint width, height, nrChannels;
    if (alpha)
    {
        internalFormat = GL_RGBA;
        imageFormat = GL_RGBA;
    }
    else
    {
        internalFormat = GL_RGB;
        imageFormat = GL_RGB;
    }
    
    // load texture data
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    
    Texture2D texture = Texture2D(width, height, internalFormat, imageFormat,GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR, data);
    
    stbi_image_free(data);
    return texture;
}

