#include "Game.hpp"

#include <iostream>

#include "ResourceManagement/ResourceManager.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "StringGlobals.hpp"
#include "Input/InputManager.hpp"

Game::Game(const Settings& settings) :
sprite_renderer_(ESpriteCentering::Center),
window_width_(settings.screen_width_),
window_height_(settings.screen_height_),
game_state_(EGameState::None)
{
    Init();
}

Game::~Game()
{}

void Game::Init()
{
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
    
    // load textures
    resourceManager.LoadTexture2D(TEXTURE1.data(), true, TEXTURE1_KEY.data());
    
    // set base shaders on renderers
    sprite_renderer_.SwapShader(spriteShader);
    text_renderer_.SwapShader(textShader);
    
    InputManager::Instance().BindInputAction(glfwGetKeyScancode(GLFW_KEY_K), &temp_input_action_);
    temp_input_action_.BindPressed([this](const UInputData& data){ this->TempInputActionPressedTest(data);});
    temp_input_action_.BindReleased([this](){ this->TempInputActionReleasedTest();});
    temp_input_action_.BindUpdated([this](const UInputData& data, const float deltaTime){ this->TempInputActionUpdateTest(data, deltaTime);});
    temp_input_action_.SetCanUpdate(true);
}

void Game::Update(const float dt)
{
    //TODO: perform game updates here
}

void Game::Render(const float dt)
{
    static float time = 0.0f;
    ResourceManager& resourceManager = ResourceManager::Instance();
    sprite_renderer_.DrawSprite(resourceManager.GetTexture2D(TEXTURE1_KEY.data()), 
        glm::vec2(250.0f, 250.0f),
        glm::vec2(400.0f, 400.0f),
        glm::sin(time));
    
    
    text_renderer_.RenderText("Ella is a DONUT!", 100.0f, 450.0f, 1.0f, glm::vec3(0.f, 1.f, 0.f));
    
    time += dt;
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
}

EGameState Game::GetGameState() const
{
    return game_state_;
}

bool Game::IsGameRunning() const
{
    return game_state_ == EGameState::InGame_Running;
}

bool Game::IsGamePaused() const
{
    return game_state_ == EGameState::InGame_Paused;
}

void Game::TempInputActionPressedTest(const UInputData& data)
{
    std::cout << "GAME::TempInputActionPressedTest called!!!!!" << std::endl;
}

void Game::TempInputActionReleasedTest()
{
    std::cout << "\nGAME::TempInputActionReleasedTest called!!!!!" << std::endl;
}

void Game::TempInputActionUpdateTest(const UInputData& data, const float deltaTime)
{
    std::cout << ".";
    
}
