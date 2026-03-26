#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP
#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include "robin_hood_hash/robin_hood.h"

#include "Components/BaseComponent.hpp"
#include "EngineDataStructures.hpp"
#include "Game/GameObject.hpp"
#include "Rendering/Shader.hpp"
#include "Rendering/Texture2D.hpp"
#include "Utils/Logger.hpp"

#include <filesystem>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <fstream>

class ResourceManager
{
    using FontMap = robin_hood::unordered_map<char, TextCharacter>;
    using FontsMap = robin_hood::unordered_map<std::string, FontMap>;
    
    robin_hood::unordered_map<std::string, Shader> shaders_;
    robin_hood::unordered_map<ESpriteKey, Texture2D> textures_;
	robin_hood::unordered_map<std::type_index, std::vector<BaseComponent*>> components_;
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
	template <typename Type>
	Type* CreateComponent(unsigned int& storageIndex, GameObject* owner); // Creates a component dynamically
	template <typename Type>
	Type* GetComponent(unsigned int storageIndex); // Returns an existing component if available
	template <typename Type>
	Type* CreateGameObject(unsigned int& storageIndex);
	template <typename Type>
	Type* GetGameObject(unsigned int storageIndex) const;
	
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

template <typename Type>
Type* ResourceManager::CreateComponent(unsigned int& storageIndex, GameObject* owner)
{
	Type* component = new Type();
	try
	{
		BaseComponent* componentPtr = dynamic_cast<BaseComponent*>(component);
		componentPtr->owner_ = owner;
		components_[typeid(Type)].push_back(componentPtr);
		storageIndex = static_cast<unsigned int>(components_[typeid(Type)].size());
	}
	catch (...)
	{
		auto log = Logger();
		log.Log(ELogLevel::Error, "ResourceManager::CreateComponent: Type was not a component!!! : [ "
				+ std::string(typeid(Type).name()) + " ]");
		delete component;
		return nullptr;
	}
	
	return component;
}

template <typename Type>
Type* ResourceManager::GetComponent(const unsigned int storageIndex)
{
	Type* result = nullptr;
	if (components_.contains(typeid(Type)) && storageIndex < static_cast<unsigned int>(components_[typeid(Type)].size()))
	{
		result = components_[typeid(Type)][storageIndex];
	}
	return result;
}

template <typename Type>
Type* ResourceManager::CreateGameObject(unsigned int& storageIndex)
{
	Type* result = new Type();
	try
	{
		GameObject* gameObject = dynamic_cast<GameObject*>(result);
		game_objects_.push_back(gameObject);
	}
	catch (...)
	{
		auto log = Logger();
		log.Log(ELogLevel::Error, "ResourceManager::CreateGameObject: Type was not a GameObject: [ "
			+ std::string(typeid(Type).name()) + " ]");
		delete result;
		return nullptr;
	}
	
	return result;
}

template <typename Type>
Type* ResourceManager::GetGameObject(const unsigned int storageIndex) const
{
	GameObject* result = nullptr;
	
	if (storageIndex < static_cast<unsigned int>(game_objects_.size()))
	{
		result = game_objects_[storageIndex];
	}
	
	return result;
}


#endif
