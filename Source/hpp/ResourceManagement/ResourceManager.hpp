#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include "Rendering/Shader.hpp"
#include "Rendering/Texture2D.hpp"
#include "EngineDataStructures.hpp"

class ResourceManager
{
public:
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    
    static ResourceManager& GetInstance();
    
    Shader* LoadShader(const char *vertexShaderFile, const char *fragmentShaderFile, const char *geometryShaderFile, const std::string& name);
    Shader* GetShader(const std::string& name);
    Texture2D* LoadTexture2D(const char *filePath, bool alpha, const std::string& name);
    Texture2D* GetTexture2D(const std::string& name);
    void Clear();
    
    Settings LoadSettings();
    Settings GetSettings() const;
    
private:
    ResourceManager();
    ~ResourceManager();
    
    // loads and generates a shader from file
    static Shader LoadShaderFromFile(const char *vertexShaderPath, const char *fragmentShaderPath, const char *geometryShaderPath = nullptr);
    // loads a single texture from file
    static Texture2D LoadTextureFromFile(const char *file, bool alpha);
    
    std::map<std::string, Shader> shaders_;
    std::map<std::string, Texture2D> textures_;
    Settings settings_;
};
#endif