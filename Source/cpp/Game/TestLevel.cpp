#include "Game/TestLevel.hpp"

#include "glm/ext/vector_float2.hpp"

#include "Utils/ResourceManager.hpp"
#include "Characters/PlayerCharacter.hpp"
#include "World/BackgroundTile.hpp"
#include "Rendering/Texture2D.hpp"
#include "EngineDataStructures.hpp"

#include <vector>

TestLevel::TestLevel()
{
	auto& resourceManager = ResourceManager::Instance();
	player_ = dynamic_cast<PlayerCharacter*>(resourceManager.CreateGameObject(EGameObjectClassType::PlayerCharacter, player_index_));
	game_objects_[1].push_back(player_);
	
	std::vector<Texture2D> debugTilesTextures;
	debugTilesTextures.push_back(resourceManager.GetTexture2D(ESpriteKey::PlaceHolder));
	unsigned int tilesIndex;
	glm::vec2 tilePositions = glm::vec2(200.0f, 200.0f);
	for (auto& debugTile : debug_tiles)
	{
		debugTile = dynamic_cast<BackgroundTile*>(resourceManager.CreateGameObject(EGameObjectClassType::BackgroundTile, tilesIndex));
		game_objects_[0].push_back(debugTile);
		debugTile->SetPosition(tilePositions);
		debugTile->SetSize(glm::vec2(100.0f));
		debugTile->SetSprite(debugTilesTextures);
		
		tilePositions += glm::vec2(100.0f);
	}
}
