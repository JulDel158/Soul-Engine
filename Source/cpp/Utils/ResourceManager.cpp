#include "Utils/ResourceManager.hpp"

#include "glad/gl.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "rapidjson/document.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/writer.h"

#include "StringGlobals.hpp"
#include "Utils/Logger.hpp"
#include "Components/SpriteComponent.hpp"
#include "Components/MovementComponent.hpp"
#include "World/BackgroundTile.hpp"

#include <filesystem>
#include <sstream>
#include <fstream>
#include <utility>
#include <cassert>

#include "Characters/Character.hpp"

ResourceManager& ResourceManager::Instance()
{
    static ResourceManager instance = ResourceManager();
    return instance;
}

ResourceManager::ResourceManager()
{
    shaders_ = robin_hood::unordered_map<std::string, Shader>();
    textures_ = robin_hood::unordered_map<ESpriteKey, Texture2D>();
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
    	auto logger = Logger();
		logger.Log(ELogLevel::Warning,"ResourceManager::LoadShader Overriding existing shader: " + name);
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

Texture2D ResourceManager::LoadTexture2D(const char* filePath, bool alpha, const ESpriteKey spriteId)
{
    if (textures_.contains(spriteId))
    {
    	auto logger = Logger();
        logger.Log(ELogLevel::Warning,"ResourceManager::LoadTexture2D: Overriding existing texture: " + DataConverter::ToString(spriteId));
        textures_[spriteId].Clear();
        textures_.erase(spriteId);
    }
    
    textures_[spriteId] = LoadTextureFromFile(filePath, alpha);
    return textures_[spriteId];
}

Texture2D ResourceManager::GetTexture2D(const ESpriteKey spriteId)
{
    return textures_[spriteId];
}

void ResourceManager::LoadFont(const char* filePath, const unsigned int fontSize, const std::string& name)
{
	auto logger = Logger();
    if (!is_ft_open_)
    {
    	logger.Log(ELogLevel::Warning, "ResourceManager::LoadFont: FT Library was not open ");
        return;
    }
    
    // load font as face
    FT_Face face;
    if (FT_New_Face(free_type_library_, filePath, 0, &face))
    {
    	logger.Log(ELogLevel::Error,"ResourceManager::LoadFont::FREETYPE: Failed to load font");
    	return;
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
        	logger.Log(ELogLevel::Error,"ResourceManager::LoadFont:FREETYPE: Failed to load Glyph!");
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
        fonts_[name][static_cast<unsigned char>(i)] = character;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // clear generated face once done, not closing FT until we are done loading fonts into memory
    FT_Done_Face(face);
}

robin_hood::unordered_map<char, TextCharacter>& ResourceManager::GetFont(const std::string& name)
{
    return fonts_[name];
}

void ResourceManager::LoadSettings(Settings& settings)
{
    std::filesystem::path settingsPath = std::filesystem::path(SETTINGS_PATH.data());
    try
    {
        std::ifstream settingsFile(settingsPath);
        std::string fileData((std::istreambuf_iterator<char>(settingsFile)),std::istreambuf_iterator<char>());
        settingsFile.close();
        
        rapidjson::Document settingsJson;
        settingsJson.Parse(fileData.c_str());
        
        if (settingsJson.HasParseError())
        {
        	auto logger = Logger();
            logger.Log(ELogLevel::Error, "ResourceManager::LoadSettings: Failed to parse settings: " +
                       std::to_string(static_cast<int>(settingsJson.GetParseError())));
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
    	auto logger = Logger();
    	logger.Log(ELogLevel::Error, "ResourceManager::LoadSettings: Failed to read Settings file " + std::string(e.what()));
    }
    
    settings = Settings(settings_);
}

void ResourceManager::SaveSettings(const Settings& settings)
{
    constexpr unsigned int BUFFER_SIZE = 65536;
    
    settings_ = Settings(settings_);
    rapidjson::Document document;
    document.SetObject();

    // Set members with settings values
    document.AddMember(static_cast<rapidjson::GenericValue<rapidjson::UTF8<>>::StringRefType>(SCREEN_WIDTH.data()), 
        settings.screen_width_, document.GetAllocator());
    document.AddMember(static_cast<rapidjson::GenericValue<rapidjson::UTF8<>>::StringRefType>(SCREEN_HEIGHT.data()), 
        settings.screen_height_, document.GetAllocator());
    document.AddMember(static_cast<rapidjson::GenericValue<rapidjson::UTF8<>>::StringRefType>(VSYNC.data()), 
        settings.vsync_, document.GetAllocator());
    document.AddMember(static_cast<rapidjson::GenericValue<rapidjson::UTF8<>>::StringRefType>(GENERAL_VOLUME.data()), 
        settings.general_volume_, document.GetAllocator());
    document.AddMember(static_cast<rapidjson::GenericValue<rapidjson::UTF8<>>::StringRefType>(MUSIC_VOLUME.data()), 
        settings.music_volume_, document.GetAllocator());
    document.AddMember(static_cast<rapidjson::GenericValue<rapidjson::UTF8<>>::StringRefType>(EFFECTS_VOLUME.data()), 
        settings.effects_volume_, document.GetAllocator());
    document.AddMember(static_cast<rapidjson::GenericValue<rapidjson::UTF8<>>::StringRefType>(DIALOGUE_VOLUME.data()), 
        settings.dialogue_volume_, document.GetAllocator());

    FILE* file;
    std::string mode;
#ifdef _WIN32
    mode = "wb";
#else 
	mode  = = "w";	
#endif
    
    if (fopen_s(&file,SETTINGS_PATH.data(), mode.c_str()) != 0)
    {
    	auto logger = Logger();
    	logger.Log(ELogLevel::Error, "ResourceManager::SaveSettings: Failed to open file: " + std::string(SETTINGS_PATH.data()));
        return;
    }
    if (!file) return;

    char buffer[BUFFER_SIZE];

    rapidjson::FileWriteStream writeStream(file, buffer, sizeof(buffer));
    rapidjson::Writer<rapidjson::FileWriteStream> writer(writeStream);
    document.Accept(writer);
    static_cast<void>(fclose(file));
}

Settings ResourceManager::GetSettings() const
{
    return settings_;
}

void ResourceManager::ReclaimFontMemory(FontMap& font, const std::string& name)
{
    fonts_[name] = std::move(font);
}

bool ResourceManager::ContainsTexture2D(const ESpriteKey spriteId) const
{
    return textures_.contains(spriteId);
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
	
	for (auto& pair : components_)
	{
		for (auto& component : pair.second)
		{
			component->Clear();
			delete component;
		}
		pair.second.clear();
	}
	
	for (const auto& object : game_objects_)
	{
		object->Clear();
		delete object;
	}
    
    shaders_.clear();
    textures_.clear();
    fonts_.clear();
	components_.clear();
	game_objects_.clear();
    
    CloseFreeTypeLibrary();
}

BaseComponent* ResourceManager::CreateComponent(const EComponentClassType type, unsigned int& storageIndex)
{
	BaseComponent* component = nullptr; // NOLINT
	
	// TODO: As component classes get created, add cases
	switch (type)
	{
	case EComponentClassType::Base:
		component = new BaseComponent();
		break;
	case EComponentClassType::SpriteComponent:
		component = new SpriteComponent();
		break;
	case EComponentClassType::MovementComponent:
	 	component = new MovementComponent();
		break;
	default: // NOLINT
		auto log = Logger();
		log.Log(ELogLevel::Error, "ResourceManager::CreateComponent: undefined or unhandled type: ["
			+ DataConverter::ToString(type) + "]");
		return nullptr;
	}
	component->SetComponenType(type);
	components_[type].push_back(component);
	storageIndex = static_cast<unsigned int>(components_[type].size());
	
	return component;
}

BaseComponent* ResourceManager::GetComponent(const EComponentClassType type, const unsigned int storageIndex)
{
	BaseComponent* result = nullptr;
	if (components_.contains(type) && storageIndex < static_cast<unsigned int>(components_[type].size()))
	{
		result = components_[type][storageIndex];
	}
	return result;
}

GameObject* ResourceManager::CreateGameObject(const EGameObjectClassType type, unsigned int& storageIndex)
{
	GameObject* result = nullptr; //NOLINT
	
	// TODO: Add cases as more classes get added to project
	switch (type)
	{
	case EGameObjectClassType::Base:
		result = new GameObject();
		break;
	case EGameObjectClassType::BackgroundTile:
		result = new BackgroundTile();
		break;
	case EGameObjectClassType::Character:
		result = new Character();
		break;
	default: // NOLINT
		auto log = Logger();
		log.Log(ELogLevel::Error, "ResourceManager::CreateComponent: undefined or unhandled type: ["
			+ DataConverter::ToString(type) + "]");
		return nullptr;
	}
	result->SetClassType(type);
	game_objects_.push_back(result);
	
	return result;
}

GameObject* ResourceManager::GetGameObject(const unsigned int storageIndex) const
{
	GameObject* result = nullptr;
	
	if (storageIndex < static_cast<unsigned int>(game_objects_.size()))
	{
		result = game_objects_[storageIndex];
	}
	
	return result;
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
    	auto logger = Logger();
    	logger.Log(ELogLevel::Error, "ResourceManager::LoadShaderFromFile: Failed to read shader files" + std::string(e.what()));
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

void ResourceManager::OpenFreeTypeLibrary()
{
	if (is_ft_open_)
	{
		return;
	}
    
	if (const auto error = FT_Init_FreeType(&free_type_library_); error)
	{
		auto logger = Logger();
		logger.Log(ELogLevel::Error, "ResourceManager::OpenFreeTypeLibrary:FREETYPE: Could not init FreeType Library!");
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
		auto logger = Logger();
		logger.Log(ELogLevel::Error,"ResourceManager::CloseFreeTypeLibrary:FREETYPE: Could not close FreeType Library");
		return;
	}
	is_ft_open_ = false;
}