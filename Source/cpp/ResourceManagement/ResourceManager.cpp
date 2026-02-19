#include "ResourceManagement/ResourceManager.hpp"

#include <glad/gl.h>

#include <iostream>
#include <filesystem>
#include <sstream>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "rapidjson/document.h"
#include "PathGlobals.hpp"

ResourceManager& ResourceManager::Instance()
{
    static ResourceManager instance = ResourceManager();
    return instance;
}

ResourceManager::ResourceManager()
{
    shaders_ = std::map<std::string, Shader>();
    textures_ = std::map<std::string, Texture2D>();
    settings_ = Settings();
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

bool ResourceManager::ContainsTexture2D(const std::string& name) const
{
    return textures_.contains(name);
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
    
    shaders_.clear();
    textures_.clear();
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

