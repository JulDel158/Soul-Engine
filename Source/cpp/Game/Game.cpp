#include "Game/Game.hpp"

#include "glad/gl.h" // NOLINT
#include "GLFW/glfw3.h"

#include "Utils/ResourceManager.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "StringGlobals.hpp"
#include "Audio/AudioManager.hpp"
#include "Input/InputManager.hpp"
#include "Rendering/SpriteRenderer.hpp"
#include "Rendering/TextRenderer.hpp"
#include "UI/Panel.hpp"
#include "Game/Level.hpp"
#include "Game/TestLevel.hpp"
#include "Combat/CombatManager.hpp"

namespace
{
	constexpr unsigned int TEXTURE_S = 0;
	constexpr unsigned int POSITION_S = 1;
	constexpr unsigned int SIZE_S = 2;
	constexpr unsigned int ROTATION_S = 3;
	constexpr unsigned int COLOR_S = 4;
	
	constexpr unsigned int TEXT_T = 0;
	constexpr unsigned int POSITION_T = 1;
	constexpr unsigned int SCALE_T = 2;
	constexpr unsigned int COLOR_T = 3;
	constexpr unsigned int FONT_T = 4;
	
}

Game::Game(const Settings& settings) :
window_width_(settings.screen_width_),
window_height_(settings.screen_height_),
window_(nullptr),
game_state_(EGameState::None),
level_(nullptr),
ui_panels_{nullptr},
in_game_run_time_(0.0f),
runtime_(0.0f)
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
	delete level_;
}

void Game::Init()
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
    resourceManager.LoadTexture2D(TEXTURE1.data(), true, ESpriteKey::Debug1);
	resourceManager.LoadTexture2D(MISSING_TEXTURE.data(), true, ESpriteKey::Missing);
	resourceManager.LoadTexture2D(PLACEHOLDER_TEXTURE.data(), true, ESpriteKey::PlaceHolder);
	resourceManager.LoadTexture2D(TEST_ANIM_FRAME_1.data(), true, ESpriteKey::Debug2);
	resourceManager.LoadTexture2D(TEST_ANIM_FRAME_2.data(), true, ESpriteKey::Debug3);
	resourceManager.LoadTexture2D(TEST_ANIM_FRAME_3.data(), true, ESpriteKey::Debug4);
	
	// TODO: Load player textures
	resourceManager.LoadTexture2D(PLAYER_IDLE_1.data(), true, ESpriteKey::Player_Idle_1);
	resourceManager.LoadTexture2D(PLAYER_IDLE_2.data(), true, ESpriteKey::Player_Idle_2);
	resourceManager.LoadTexture2D(PLAYER_IDLE_3.data(), true, ESpriteKey::Player_Idle_3);
	
	resourceManager.LoadTexture2D(PLAYER_WALKING_LEFT_1.data(), true, ESpriteKey::Player_Left_1);
	resourceManager.LoadTexture2D(PLAYER_WALKING_LEFT_2.data(), true, ESpriteKey::Player_Left_2);
	resourceManager.LoadTexture2D(PLAYER_WALKING_LEFT_3.data(), true, ESpriteKey::Player_Left_3);
	
	resourceManager.LoadTexture2D(PLAYER_WALKING_RIGHT_1.data(), true, ESpriteKey::Player_Right_1);
	resourceManager.LoadTexture2D(PLAYER_WALKING_RIGHT_2.data(), true, ESpriteKey::Player_Right_2);
	resourceManager.LoadTexture2D(PLAYER_WALKING_RIGHT_3.data(), true, ESpriteKey::Player_Right_3);
	
	resourceManager.LoadTexture2D(PLAYER_WALKING_UP_1.data(), true, ESpriteKey::Player_Up_1);
	resourceManager.LoadTexture2D(PLAYER_WALKING_UP_2.data(), true, ESpriteKey::Player_Up_2);
	resourceManager.LoadTexture2D(PLAYER_WALKING_UP_3.data(), true, ESpriteKey::Player_Up_3);
	
	resourceManager.LoadTexture2D(PLAYER_WALKING_DOWN_1.data(), true, ESpriteKey::Player_Down_1);
	resourceManager.LoadTexture2D(PLAYER_WALKING_DOWN_2.data(), true, ESpriteKey::Player_Down_2);
	resourceManager.LoadTexture2D(PLAYER_WALKING_DOWN_3.data(), true, ESpriteKey::Player_Down_3);
	
	resourceManager.LoadTexture2D(BUTTON_1.data(), true, ESpriteKey::Button1);
	resourceManager.LoadTexture2D(BUTTON_1_FOCUSED.data(), true, ESpriteKey::Button1_Focused);
	resourceManager.LoadTexture2D(BUTTON_1_PRESSED.data(), true, ESpriteKey::Button1_Pressed);
    
    // set base shaders on renderers
    sprite_renderer_->SwapShader(spriteShader);
    text_renderer_->SwapShader(textShader);
	
	level_ = new TestLevel();
	game_state_ = EGameState::InGame_Running;
	
	auto& combatManager = CombatManager::Instance();
	combatManager.Initialize();
	SetPanel(combatManager.GetUIPanel(), static_cast<unsigned int>(EPanelType::Game_1));
}

void Game::Start()
{
	// for (const auto& gameObject : game_objects_)
	// {
	// 	gameObject->Start();
	// }
	if (level_ != nullptr)
	{
		level_->Start();
	}
	
	// after updating game objects, we update the UI
	for (const auto& panel : ui_panels_)
	{
		if (panel == nullptr)
		{
			continue;
		}
		
		panel->Start();
	}
}

void Game::Update(const float dt)
{
	runtime_ += dt;
	if (game_state_ == EGameState::InGame_Running)
	{
		// TODO: Game objects will now be updated from within the owning level
		// for (const auto& gameObject : game_objects_)
		// {
		// 	if (!gameObject->IsActive())
		// 	{
		// 		continue;
		// 	}
		//
		// 	gameObject->Update(dt);
		// }
		if (level_ != nullptr)
		{
			level_->Update(dt);
		}
		in_game_run_time_ += dt;
	}
	
	int startIndex = 1;
	int endIndex = 0;
	
	// We will update UI based on the game state
	GetUIPanelIndex(startIndex, endIndex);
	
	for (int i = startIndex; i <= endIndex; i++)
	{
		if (ui_panels_[i] == nullptr || !ui_panels_[i]->IsActive())
		{
			continue;
		}
		
		ui_panels_[i]->Update(dt);
	}
}

void Game::Render(const float dt) const
{
    static float runTime = 0.0f;
    
	if (true) 
	{ // Temporary
		ResourceManager& resourceManager = ResourceManager::Instance();
		sprite_renderer_->DrawSprite(resourceManager.GetTexture2D(ESpriteKey::Debug1), 
			glm::vec2(600.0f + glm::sin(runTime) * 400.0f, 500.0f),
			glm::vec2(400.0f, 400.0f),
			(glm::cos(runTime) + std::sin(runTime)));
    
    	text_renderer_->RenderText("Sample Text", 
    		glm::vec2(0.0f, 0.0f), 
    		3.0f, 
    		glm::vec3(0.1f, glm::clamp(glm::cos(runTime), 0.f, 1.f), glm::clamp(glm::sin(runTime), 0.f, 1.f)));
	}

	// Drawing all targets within all game objects
	if (level_ != nullptr)
	{
		level_->Render(dt, *sprite_renderer_);
	}
	
   //  for (const auto& gameObject : game_objects_)
   //  {
   //  	if (!gameObject->IsVisible())
   //  	{
   //  		continue;
   //  	}
   //  	
   //  	for (const auto& renderTarget : gameObject->GetRenderList())
   //  	{
			// sprite_renderer_->DrawSprite(std::get<SHADER_S>(renderTarget), 
			// 	std::get<POSITION_S>(renderTarget), 
			// 	std::get<SIZE_S>(renderTarget), 
			// 	std::get<ROTATION_S>(renderTarget), 
			// 	std::get<COLOR_S>(renderTarget));
   //  	}
   //  }
	
	int startIndex = 1;
	int endIndex = 0;
	
	GetUIPanelIndex(startIndex, endIndex);
	
	for (int i = startIndex; i <= endIndex; i++)
	{
		if (ui_panels_[i] == nullptr || !ui_panels_[i]->IsVisible())
		{
			continue;
		}
		
		for (const auto& widgets : ui_panels_[i]->widgets_) // NOLINT
		{
			for (auto& widget : widgets.second)
			{
				if (!widget->IsVisible())
				{
					continue;
				}
				
				// first we render sprites
				for (const auto& renderTarget : widget->GetRenderList())
				{
					sprite_renderer_->DrawSprite(
						std::get<TEXTURE_S>(renderTarget), 
				std::get<POSITION_S>(renderTarget), 
				std::get<SIZE_S>(renderTarget), 
				std::get<ROTATION_S>(renderTarget), 
				std::get<COLOR_S>(renderTarget));
				}
				
				// Now we render the text as well
				for (const auto& textTarget : widget->GetTextList())
				{
					text_renderer_->RenderText(
						std::get<TEXT_T>(textTarget),
						std::get<POSITION_T>(textTarget),
						std::get<SCALE_T>(textTarget),
						std::get<COLOR_T>(textTarget),
						std::get<FONT_T>(textTarget));
				}
			}
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
	// for (const auto& gameObject : game_objects_)
	// {
	// 	gameObject->End();
	// }
	if (level_ != nullptr)
	{
		level_->End();
	}
	
	// after updating game objects, we update the UI
	for (const auto& panel : ui_panels_)
	{
		if (panel == nullptr)
		{
			continue;
		}
		
		panel->End();
	}
	
	game_state_ = EGameState::Unloading;
}

void Game::SetWindowPointer(GLFWwindow* window)
{
    window_ = window;
}

void Game::SetPanel(Panel* panel, unsigned int index)
{
	if (index < static_cast<unsigned int>(EPanelType::Count))
	{
		ui_panels_[index] = panel;
	}
}

void Game::GetUIPanelIndex(int& startIndex, int& endIndex) const
{
	switch (game_state_) // NOLINT
	{
	case EGameState::MainMenu:
		{
			endIndex = startIndex = static_cast<int>(EPanelType::MainMenu);
			break;
		}
	case EGameState::Settings:
		{
			endIndex = startIndex = static_cast<int>(EPanelType::Settings_Main);
			break;
		}
	case EGameState::Credits:
		{
			endIndex = startIndex = static_cast<int>(EPanelType::Credits);
			break;
		}
	case EGameState::InGame_Running:
		{
			startIndex = static_cast<int>(EPanelType::Game_1);
			endIndex = static_cast<int>(EPanelType::Game_10);
			break;
		}
	case EGameState::InGame_Paused:
		{
			endIndex = startIndex = static_cast<int>(EPanelType::Pause);
			break;
		}
	case EGameState::InGame_Settings:
		{
			endIndex = startIndex = static_cast<int>(EPanelType::Settings_InGame);
			break;
		}
	case EGameState::Loading:
	case EGameState::Unloading:
		{
			startIndex = static_cast<int>(EPanelType::Loading_1);
			endIndex = static_cast<int>(EPanelType::Loading_9);
			break;
		}
	default:
		startIndex = static_cast<int>(EPanelType::Count);
		endIndex = 0;
		break;
	}
}
