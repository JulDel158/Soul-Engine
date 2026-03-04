#ifndef GAME_H
#define GAME_H

#include "glad/gl.h" // NOLINT
#include "GLFW/glfw3.h"

#include "EngineDataStructures.hpp"
#include "Rendering/SpriteRenderer.hpp"
#include "Rendering/TextRenderer.hpp"
#include "GameObject.hpp"

#include <vector>

// Class in charge of game's core. From here we will perform updates to the game state, physics, rendering, audio, and more.
class Game
{
private:
    SpriteRenderer sprite_renderer_;
    EGameState game_state_;
    TextRenderer text_renderer_;
    unsigned int window_width_;
    unsigned int window_height_;
    GLFWwindow* window_;
	std::vector<GameObject*> game_objects_;
	
	// Texture, position, size, rotation, color
	//std::vector<std::tuple<Texture2D, glm::vec2, glm::vec2, float, glm::vec3>> render_list_;
	
    
    void Init();
    
public:
    
    Game()=delete;
    explicit Game(const Settings& settings);
    ~Game();
    
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    
    void Update(const float dt);
    void Render(const float dt) const;
    void ProcessAudio(const float dt);
    void ProcessInput(const float dt);
    void End();
    
    EGameState GetGameState() const;
    bool IsGameRunning() const;
    bool IsGamePaused() const;
    
    void SetWindowPointer(GLFWwindow* window);
};



#endif