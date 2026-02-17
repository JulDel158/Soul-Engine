#include "Game.hpp"

#include "ResourceManagement/ResourceManager.hpp"
#include "glm/gtc/matrix_transform.hpp"

Game::Game(Settings settings) :
window_width_(settings.screen_width_),
window_height_(settings.screen_height_),
game_state_(EGameState::None),
keys_(),
keys_previous_()
{
}

Game::~Game()
{
    delete sprite_renderer_;
}

void Game::Init()
{
    ResourceManager& resourceManager = ResourceManager::GetInstance();
    resourceManager.LoadShader("Rendering/Shaders/BasicSprite.vert", 
        "Rendering/Shaders/BasicSprite.frag", 
        nullptr, "sprite");
    
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(window_width_), 
        static_cast<float>(window_height_), 0.0f, -1.0f, 1.0f);
    
    resourceManager.GetShader("sprite").Use();
    resourceManager.GetShader("sprite").SetUniformInteger("image", 0);
    resourceManager.GetShader("sprite").SetUniformMatrix4("projection", projection);
    
    sprite_renderer_ = new SpriteRenderer(resourceManager.GetShader("sprite"));
    resourceManager.LoadTexture2D("Assets/Textures/awesomeface.png", true, "face");
}

void Game::Update(float dt)
{
}

void Game::Render(float dt)
{
    ResourceManager& resourceManager = ResourceManager::GetInstance();
    sprite_renderer_->DrawSprite(resourceManager.GetTexture2D("face"), 
        glm::vec2(200.0f, 200.0f),
        glm::vec2(300.0f, 400.0f),
        45.0f, 
        glm::vec3(0.0f, 0.0f, 0.0f));
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
