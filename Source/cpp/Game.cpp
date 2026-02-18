#include "Game.hpp"

#include <iostream>
#include <ostream>

#include "ResourceManagement/ResourceManager.hpp"
#include "glm/gtc/matrix_transform.hpp"

Game::Game(const Settings& settings) :
window_width_(settings.screen_width_),
window_height_(settings.screen_height_),
sprite_renderer_(ESpriteCentering::CENTER),
game_state_(EGameState::None),
keys_(),
keys_previous_()
{
    Init();
}

Game::~Game()
{
    std::cout<<"Game Destructor"<<std::endl;
}

void Game::Init()
{
    ResourceManager& resourceManager = ResourceManager::GetInstance();
    resourceManager.LoadShader("Assets/Shaders/BasicSprite.vert", 
        "Assets/Shaders/BasicSprite.frag", 
        nullptr, "sprite");
    
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(window_width_), 
        static_cast<float>(window_height_), 0.0f, -1.0f, 1.0f);
    
    resourceManager.GetShader("sprite").Use().SetUniformInteger("image", 0);
    resourceManager.GetShader("sprite").SetUniformMatrix4("projection", projection);
    
    sprite_renderer_.SwapShader(resourceManager.GetShader("sprite"));
    resourceManager.LoadTexture2D("D:/Projects/Soul/Assets/Textures/awesomeface.png", true, "face");
}

void Game::Update(float dt)
{
}

void Game::Render(float dt)
{
    static float time = 0.0f;
    ResourceManager& resourceManager = ResourceManager::GetInstance();
    sprite_renderer_.DrawSprite(resourceManager.GetTexture2D("face"), 
        glm::vec2(250.0f, 250.0f),
        glm::vec2(400.0f, 400.0f),
        glm::sin(time));
    time += dt;
}

void Game::ProcessAudio(const float dt)
{
}

void Game::ProcessInput(const float dt)
{
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
