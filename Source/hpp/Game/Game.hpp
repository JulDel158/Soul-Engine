#ifndef GAME_HPP
#define GAME_HPP
#pragma once

#include "EngineDataStructures.hpp"

#include <vector>

struct GLFWwindow; //NOLINT
class SpriteRenderer;
class TextRenderer;
class GameObject;
class Panel;

// Class in charge of game's core. From here we will perform updates to the game state, physics, rendering, audio.
// From here it is communicated what gets loaded and unloaded and the state of game objects
class Game
{
private:
    SpriteRenderer* sprite_renderer_;
    TextRenderer* text_renderer_;
    unsigned int window_width_;
    unsigned int window_height_;
    GLFWwindow* window_;
    EGameState game_state_;
	std::vector<GameObject*> game_objects_;
	Panel* ui_panels_[static_cast<unsigned int>(EPanelType::Count)];
    
    void Init() const;
    
public:
    explicit Game(const Settings& settings);
    ~Game();
    Game()=delete;
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    
	void Start();
    void Update(const float dt);
    void Render(const float dt) const;
    void ProcessAudio(const float dt);
    void ProcessInput(const float dt);
    void End();
    
	inline EGameState	GetGameState() const	{ return game_state_; }
    inline bool			IsGameRunning() const	{ return game_state_ == EGameState::InGame_Running; }
    inline bool			IsGamePaused() const	{ return game_state_ == EGameState::InGame_Paused; }
    
    void SetWindowPointer(GLFWwindow* window);
	
private:
	void GetUIPanelIndex(int& startIndex, int& endIndex) const;
};
#endif