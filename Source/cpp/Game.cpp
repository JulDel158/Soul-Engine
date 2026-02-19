#include "Game.hpp"

#include <iostream>
#include <ostream>

#include "ResourceManagement/ResourceManager.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "PathGlobals.hpp"

Game::Game(const Settings& settings) :
sprite_renderer_(ESpriteCentering::Center),
window_width_(settings.screen_width_),
window_height_(settings.screen_height_),
game_state_(EGameState::None)
{
    Init();
}

Game::~Game()
{
    std::cout<<"Game Destructor"<<std::endl;
}

void Game::Init()
{
    ResourceManager& resourceManager = ResourceManager::Instance();
    Shader shader = resourceManager.LoadShader(
        VERTEX_SHADER1.data(), FRAGMENT_SHADER1.data(), // NOLINT (String contains null terminator)
        nullptr, "sprite"); 
    
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(window_width_), 
        static_cast<float>(window_height_), 0.0f, -1.0f, 1.0f);
    
    shader.Use();
    shader.SetUniformInteger("image", 0);
    shader.SetUniformMatrix4("projection", projection);
    
    // Must set a shader on the sprite renderer
    sprite_renderer_.SwapShader(resourceManager.GetShader("sprite"));
    resourceManager.LoadTexture2D(TEXTURE1.data(), true, "face"); // NOLINT (string contains nullterminator)
}

void Game::Update(const float dt)
{
    //TODO: perform game updates here
}

void Game::Render(const float dt)
{
    static float time = 0.0f;
    ResourceManager& resourceManager = ResourceManager::Instance();
    sprite_renderer_.DrawSprite(resourceManager.GetTexture2D("face"), 
        glm::vec2(250.0f, 250.0f),
        glm::vec2(400.0f, 400.0f),
        glm::sin(time));
    time += dt;
}

void Game::ProcessAudio(const float dt)
{
    // TODO: Process audio here
}

void Game::ProcessInput(const float dt)
{
    // TODO: Process input here
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
