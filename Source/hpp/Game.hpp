#ifndef GAME_H
#define GAME_H

#define KEYS_COUNT 1024

#include "EngineDataStructures.hpp"

enum class EGameState : unsigned char
{
    None = 0,
    MainMenu = 1,
    Settings = 2,
    
    InGame_Running = 3,
    InGame_Paused = 4,
    InGame_Loading = 5,
    
    Exiting = 6
};

// Class in charge of game's core. From here we will perform updates to the game state, physics, rendering, audio, and more.
class Game
{
private:
    
    unsigned int window_width_;
    unsigned int window_height_;
    EGameState game_state_;
    bool keys_[KEYS_COUNT];
    bool keys_previous_[KEYS_COUNT];
    
public:
    
    Game()=delete;
    Game(Settings settings);
    ~Game();
    
    void Init();
    void Update(const float dt);
    void Render(const float dt);
    void ProcessAudio(const float dt);
    void ProcessInput(const float dt);
    void End();
    
    EGameState GetGameState() const;
    bool IsGameRunning() const;
    bool IsGamePaused() const;
};



#endif