#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP
#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include "robin_hood_hash/robin_hood.h"

#include "Rendering/Shader.hpp"
#include "Rendering/Texture2D.hpp"
#include "EngineDataStructures.hpp"
#include "Components/BaseComponent.hpp"
#include "Game/GameObject.hpp"

#include <filesystem>
#include <string>

class ResourceManager
{
    using FontMap = robin_hood::unordered_map<char, TextCharacter>;
    using FontsMap = robin_hood::unordered_map<std::string, FontMap>;
    
    robin_hood::unordered_map<std::string, Shader> shaders_;
    robin_hood::unordered_map<ESpriteKey, Texture2D> textures_;
	robin_hood::unordered_map<EComponentClassType, std::vector<BaseComponent*>> components_;
	std::vector<GameObject*> game_objects_;
    FontsMap fonts_;
    FT_Library free_type_library_;
    Settings settings_;
    bool is_ft_open_;
    
public:
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    
    static ResourceManager& Instance();
    
    Shader LoadShader(const char *vertexShaderFile, const char *fragmentShaderFile, const char *geometryShaderFile, const std::string& name);
    Shader GetShader(const std::string& name);
    
    Texture2D LoadTexture2D(const char *filePath, bool alpha, const ESpriteKey spriteId);
    Texture2D GetTexture2D(const ESpriteKey spriteId);
    
    void LoadFont(const char* filePath, const unsigned int fontSize, const std::string& name);
    FontMap& GetFont(const std::string& name);
    
    void LoadSettings(Settings& settings);
    void SaveSettings(const Settings& settings);
    Settings GetSettings() const;

    // Text renderer moves memory back and forth when using a font.
    // This function returns the memory to the resource manager
    void ReclaimFontMemory(FontMap& font, const std::string& name);
    
    bool ContainsShader(const std::string& name) const;
    bool ContainsTexture2D(const ESpriteKey spriteId) const;
    bool ContainsFont(const std::string& name) const;
	
	// Level loading
	BaseComponent& CreateComponent(EComponentClassType type, unsigned int& storageIndex); // Creates a component dynamically
	BaseComponent* GetComponent(EComponentClassType type, unsigned int storageIndex); // Returns an existing component if available
	GameObject& CreateGameObject(EGameObjectClassType type, unsigned int& storageIndex);
	GameObject* GetGameObject(unsigned int storageIndex) const;
	
	void OpenFreeTypeLibrary(); // call before loading fonts. Note: this is called on construction
	void CloseFreeTypeLibrary(); // call after we finish loading fonts
    
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