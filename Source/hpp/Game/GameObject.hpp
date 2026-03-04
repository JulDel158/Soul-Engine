#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
#include "Rendering/Texture2D.hpp"

#include <tuple>
#include <vector>

class BaseComponent;

// This class represents the base form of all objects that are part of the game,
// The game class will have a list of all loaded objects and send updates as well as process rendering for any sprites they contain
class GameObject
{
	using RenderList = std::vector<std::tuple<Texture2D, glm::vec2, glm::vec2, float, glm::vec3>>;
	friend class Game;
	friend class BaseComponent;
	
private:
	glm::vec2 position_;
	glm::vec2 size_;
	float rotation_;
	std::vector<BaseComponent*> components_;
	RenderList render_list_;
	bool is_active_;
	bool is_visible_;
	
public:
	GameObject();
	virtual ~GameObject();
	
	inline const RenderList& GetRenderList() const {return render_list_;}
	void AddComponent(BaseComponent* component);
	
	inline void SetIsActive(const bool isActive) { is_active_ = isActive; }
	inline void SetIsVisible(const bool isVisible) { is_visible_ = isVisible; }
	
	inline bool IsActive() const { return is_active_; }
	inline bool IsVisible() const { return is_visible_; }
	
protected:
	virtual void Init();
	virtual void Update(const float deltaTime);
};

#endif
