#include "Game/TestLevel.hpp"

#include "Utils/ResourceManager.hpp"
#include "Characters/PlayerCharacter.hpp"

TestLevel::TestLevel()
{
	auto& resourceManager = ResourceManager::Instance();
	player_ = dynamic_cast<PlayerCharacter*>(resourceManager.CreateGameObject(EGameObjectClassType::PlayerCharacter, player_index_));
	game_objects_[0].push_back(player_);
}
