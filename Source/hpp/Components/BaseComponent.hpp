#ifndef BASE_COMPONENT_H
#define BASE_COMPONENT_H

#include "glm/ext/vector_float2.hpp"

// Base class for all game components
class BaseComponent
{
protected:
	glm::vec2 position_;
	glm::vec2 size_;
	glm::vec2 rotation_;
	bool enabled_;
	
public:
	BaseComponent();
	virtual ~BaseComponent() = default;
	
protected:
	virtual void Init();
	virtual void Update(const float deltaTime);
	
	inline void SetPosition(const glm::vec2& position) { position_ = position; }
	inline void SetSize(const glm::vec2& size) { size_ = size; }
	inline void SetRotation(const glm::vec2& rotation) { rotation_ = rotation; }
	inline void SetEnabled(const bool enabled) { enabled_ = enabled; }
	
	inline glm::vec2 GetPosition() const { return position_; }
	inline glm::vec2 GetSize() const { return size_; }
	inline glm::vec2 GetRotation() const { return rotation_; }
	inline bool GetEnabled() const { return enabled_; }
};

#endif
