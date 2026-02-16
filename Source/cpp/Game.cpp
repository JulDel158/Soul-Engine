#include "Game.hpp"

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
}

void Game::Init()
{
}

void Game::Update(float dt)
{
}

void Game::Render(float dt)
{
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
