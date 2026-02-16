#include "Input/InputManager.hpp"

InputManager& InputManager::GetInstance()
{
    static InputManager instance = InputManager();
    return instance;
}

void InputManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
}

void InputManager::SetGameInstance(const Game& game)
{
}

InputManager::InputManager():
game_instance_(nullptr)
{
}

InputManager::~InputManager()
{
    game_instance_ = nullptr;
}
