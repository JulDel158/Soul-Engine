#include "Game/Level.hpp"

#include "Game/GameObject.hpp"
#include "Rendering/SpriteRenderer.hpp"

namespace
{
	constexpr unsigned int TEXTURE_S = 0;
	constexpr unsigned int POSITION_S = 1;
	constexpr unsigned int SIZE_S = 2;
	constexpr unsigned int ROTATION_S = 3;
	constexpr unsigned int COLOR_S = 4;
}

Level::Level()
{
	// TODO: Load level assets
}

Level::~Level()
{
	// TODO: Tell resource manager to free memory or recycle it
}

void Level::Start()
{
	for (const auto& layer : game_objects_) // NOLINT
	{
		for (const auto& gameObject : layer.second)
		{
			gameObject->Start();
		}
	}
}

void Level::Update(const float deltaTime)
{
	for (const auto& layer : game_objects_) // NOLINT
	{
		for (const auto& gameObject : layer.second)
		{
			gameObject->Update(deltaTime);
		}
	}
}

void Level::End()
{
	for (const auto& layer : game_objects_) // NOLINT
	{
		for (const auto& gameObject : layer.second)
		{
			gameObject->End();
		}
	}
}

void Level::Render(const float deltaTime, SpriteRenderer& renderer)
{
	for (const auto& layer : game_objects_) // NOLINT
	{
		for (const auto& gameObject : layer.second)
		{
			for (const auto& renderTarget : gameObject->GetRenderList())
			{
				renderer.DrawSprite(std::get<TEXTURE_S>(renderTarget), 
				std::get<POSITION_S>(renderTarget), 
				std::get<SIZE_S>(renderTarget), 
				std::get<ROTATION_S>(renderTarget), 
				std::get<COLOR_S>(renderTarget));
			}
		}
	}
}
