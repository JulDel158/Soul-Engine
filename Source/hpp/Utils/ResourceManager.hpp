#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <filesystem>
#include <unordered_map>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Rendering/Shader.hpp"
#include "Rendering/Texture2D.hpp"
#include "EngineDataStructures.hpp"

class ResourceManager
{
    using InnerMap = std::unordered_map<char, TextCharacter>;
    using OuterMap = std::unordered_map<std::string, InnerMap>;
    
    std::unordered_map<std::string, Shader> shaders_;
    std::unordered_map<std::string, Texture2D> textures_;
    OuterMap fonts_;
    FT_Library free_type_library_;
    Settings settings_;
    bool is_ft_open_;
    
public:
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    
    static ResourceManager& Instance();
    
    Shader LoadShader(const char *vertexShaderFile, const char *fragmentShaderFile, const char *geometryShaderFile, const std::string& name);
    Shader GetShader(const std::string& name);
    
    Texture2D LoadTexture2D(const char *filePath, bool alpha, const std::string& name);
    Texture2D GetTexture2D(const std::string& name);
    
    void LoadFont(const char* filePath, const unsigned int fontSize, const std::string& name);
    InnerMap& GetFont(const std::string& name);
    
    void LoadSettings(Settings& settings);
    void SaveSettings(const Settings& settings);
    Settings GetSettings() const;

    // Text renderer moves memory back and forth when using a font.
    // This function returns the memory to the resource manager
    void ReclaimFontMemory(InnerMap& font, const std::string& name);
    
    bool ContainsShader(const std::string& name) const;
    bool ContainsTexture2D(const std::string& name) const;
    bool ContainsFont(const std::string& name) const;
    
    // call before loading fonts. Note: this is called on construction
    void OpenFreeTypeLibrary();
    
    // call after we finish loading fonts
    void CloseFreeTypeLibrary();
    
private:
    ResourceManager();
    ~ResourceManager();
    
    void Clear();
    
    static Shader LoadShaderFromFile(const std::filesystem::path& vertexShaderPath, 
        const std::filesystem::path& fragmentShaderPath, const std::filesystem::path& geometryShaderPath = "", 
        bool hasGeometryShader = false);
    
    static Texture2D LoadTextureFromFile(const char* file, bool alpha);
};
#endif