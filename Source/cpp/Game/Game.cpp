#include "Game/Game.hpp"

#include "glad/gl.h" // NOLINT
#include "GLFW/glfw3.h"

#include "Utils/ResourceManager.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "StringGlobals.hpp"
#include "Input/InputManager.hpp"
#include "Rendering/SpriteRenderer.hpp"
#include "Rendering/TextRenderer.hpp"
#include "Game/GameObject.hpp"

namespace
{
	constexpr unsigned int SHADER_T = 0;
	constexpr unsigned int POSITION_T = 1;
	constexpr unsigned int SIZE_T = 2;
	constexpr unsigned int ROTATION_T = 3;
	constexpr unsigned int COLOR_T = 4;
	
}

Game::Game(const Settings& settings) :
window_width_(settings.screen_width_),
window_height_(settings.screen_height_),
window_(nullptr),
game_state_(EGameState::None)
{
	sprite_renderer_ = new SpriteRenderer(ESpriteCentering::Center);
	text_renderer_ = new TextRenderer();
    Init();
}

Game::~Game()
{
    End();
	
	delete text_renderer_;
	delete sprite_renderer_;
}

void Game::Init() const
{
	// Initialize/Load all persistent data, such as fonts and certain images
    ResourceManager& resourceManager = ResourceManager::Instance();
    
    // load shaders
    const Shader spriteShader = resourceManager.LoadShader(
        V_SPRITE_SHADER_BASE.data(), F_SPRITE_SHADER_BASE.data(),
        nullptr, SPRITE_RENDERER_PROGRAM1.data()); 
    
    const Shader textShader = resourceManager.LoadShader(V_TEXT_SHADER_BASE.data(),  
        F_TEXT_SHADER_BASE.data(), nullptr, FONT_RENDERER_PROGRAM1.data());
    
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(window_width_), 
        static_cast<float>(window_height_), 0.0f, -1.0f, 1.0f);
    
    spriteShader.SetUniformInteger(IMAGE_UNIFORM.data(), 0, true);
    spriteShader.SetUniformMatrix4(PROJECTION_UNIFORM.data(), projection);
    
    projection = glm::ortho(0.0f, static_cast<float>(window_width_), static_cast<float>(window_height_), 0.0f);
    textShader.SetUniformMatrix4(PROJECTION_UNIFORM.data(), projection, true);
    textShader.SetUniformInteger(TEXT_UNIFORM.data(), 0);
    
    // load fonts
    resourceManager.OpenFreeTypeLibrary();
    resourceManager.LoadFont(FONT_ARIAL_PATH.data(), 24,FONT_ARIAL.data());
    //close ft library once we are done loading fonts
    resourceManager.CloseFreeTypeLibrary();
    
    // load textures. Note: Texture loading may be moved over time and loaded/unloaded based on "levels"
    resourceManager.LoadTexture2D(TEXTURE1.data(), true, TEXTURE1_KEY.data());
	resourceManager.LoadTexture2D(MISSING_TEXTURE.data(), true, MISSING_TEXTURE_KEY.data());
	resourceManager.LoadTexture2D(PLACEHOLDER_TEXTURE.data(), true, PLACEHOLDER_TEXTURE_KEY.data());
	resourceManager.LoadTexture2D(TEST_ANIM_FRAME_1.data(), true, ANIM_KEY_1.data());
	resourceManager.LoadTexture2D(TEST_ANIM_FRAME_2.data(), true, ANIM_KEY_2.data());
	resourceManager.LoadTexture2D(TEST_ANIM_FRAME_3.data(), true, ANIM_KEY_3.data());
    
    // set base shaders on renderers
    sprite_renderer_->SwapShader(spriteShader);
    text_renderer_->SwapShader(textShader);
}

void Game::Update(const float dt)
{
	if (!IsGameRunning())
	{
		return;
	}
	
	for (const auto& gameObject : game_objects_)
	{
		if (!gameObject->IsActive())
		{
			continue;
		}
		
		gameObject->Update(dt);
	}
}

void Game::Render(const float dt) const
{
    static float runTime = 0.0f;
    
	if (true) 
	{ // Temporary
		ResourceManager& resourceManager = ResourceManager::Instance();
		sprite_renderer_->DrawSprite(resourceManager.GetTexture2D(TEXTURE1_KEY.data()), 
			glm::vec2(600.0f + glm::sin(runTime) * 400.0f, 500.0f),
			glm::vec2(400.0f, 400.0f),
			(glm::cos(runTime) + std::sin(runTime)));
    
    	text_renderer_->RenderText("Sample Text", glm::vec2(200.0f, 100.0f), 3.0f, glm::vec3(
			0.1f, 
			glm::clamp(glm::cos(runTime), 0.f, 1.f), 
			 glm::clamp(glm::sin(runTime), 0.f, 1.f)));
	}

	// Drawing all targets within all game objects
    for (const auto& gameObject : game_objects_)
    {
    	if (!gameObject->IsVisible())
    	{
    		continue;
    	}
    	
    	for (const auto& renderTarget : gameObject->GetRenderList())
    	{
			sprite_renderer_->DrawSprite(std::get<SHADER_T>(renderTarget), 
				std::get<POSITION_T>(renderTarget), 
				std::get<SIZE_T>(renderTarget), 
				std::get<ROTATION_T>(renderTarget), 
				std::get<COLOR_T>(renderTarget));
    	}
    }
    
    runTime += dt;
}

void Game::ProcessAudio(const float dt)
{
    // TODO: Process audio here
}

void Game::ProcessInput(const float dt)
{
    InputManager::Instance().InputUpdate(dt);
}

void Game::End()
{
	for (const auto& gameObject : game_objects_)
	{
		gameObject->End();
	}
	
	game_state_ = EGameState::Unloading;
}

void Game::SetWindowPointer(GLFWwindow* window)
{
    window_ = window;
}
