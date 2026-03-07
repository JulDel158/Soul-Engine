#pragma once

#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"

#include "EngineDataStructures.hpp"
#include "Rendering/Texture2D.hpp"

#include <vector>
#include <tuple>
#include <string>

class Widget
{
protected:
	friend class Panel;
	using RenderData = std::tuple<Texture2D, glm::vec2, glm::vec2, float, glm::vec3>;
	using RenderList = std::vector<RenderData>;
	using TextData = std::tuple<std::string, glm::vec2, float, glm::vec3, std::string>;
	using TextList = std::vector<TextData>;
	
	Widget* parent_;
	std::vector<Widget*> children_;
	Widget* neighbors_[static_cast<unsigned int>(EWidgetNeighbor::Count)];
	
	RenderList render_list_;
	TextList text_list_;
	
	glm::vec2 position_;
	glm::vec2 size_;
	glm::vec2 scale_;
	float rotation_;
	unsigned int layer_;
	bool is_active_;
	bool is_visible_;
	bool is_highlighted_;
	bool can_update_;
	bool can_refresh_;
	
public:
	Widget();
	virtual ~Widget();
	
protected:
	virtual void Start();
	virtual void Update(const float deltaTime);
	virtual void End();
	
public:
	// Input events
	virtual void OnMouseEnter(); // Event triggered when cursor hovers over widget
	virtual void OnMouseLeave(); // Event triggered when cursors stops hovering over widget
	virtual void OnFocused(); // Event triggered when this widget becomes focused  via keyboard or controller
	virtual void OnUnfocused(); // Event triggered when this widget loses focus via controller or keyboard
	virtual void OnClick(); // Event triggered when mouse is clicked while hovering over widget
	virtual void OnSelected(); // Event triggered when widget is focused and selected via input (ex. pressing enter on keyboard or 'A'/'Start' on controller
	
	// Setters
	void SetParent(Widget& parent);
	void AddChild(Widget& child);
	void AddNeighbor(Widget& neighbor, EWidgetNeighbor direction);
	
	inline void SetPosition(const glm::vec2 position)	{ position_ = position; }
	inline void SetSize(const glm::vec2 size)			{ size_ = size; }
	inline void SetScale(const glm::vec2 scale)			{ scale_ = scale; }
	inline void SetRotation(const float rotation)		{ rotation_ = rotation; }
	inline void SetLayer(const unsigned int layer)		{ layer_ = layer; }
	inline void SetActive(const bool active)			{ is_active_ = active; }
	inline void SetVisible(const bool visible)			{ is_visible_ = visible; }
	inline void SetCanUpdate(const bool update)			{ can_update_ = update; }
	inline void SetRefresh(const bool refresh)			{ can_refresh_ = refresh; }
	
	// Getters
	inline Widget* GetParent() const								{ return parent_; }
	inline std::vector<Widget*> GetChildren() const					{ return children_; }
	inline Widget* GetNeighbor(EWidgetNeighbor direction) const 	{ return neighbors_[static_cast<unsigned int>(direction)]; }
	
	inline const RenderList& GetRenderList() const					{ return render_list_; }
	inline const TextList& GetTextList() const						{ return text_list_; }
	
	inline glm::vec2	GetPosition() const 						{ return position_; }
	inline glm::vec2	GetSize() const								{ return size_; }
	inline glm::vec2	GetScale() const							{ return scale_; }
	inline float		GetRotation() const 						{ return rotation_; }
	inline unsigned int	GetLayer() const							{ return layer_; }
	inline bool			IsActive() const							{ return is_active_; }
	inline bool			IsVisible() const							{ return is_visible_; }
	inline bool			IsHighlighted() const						{ return is_highlighted_; }
	inline bool			CanUpdate() const							{ return can_update_; }
	inline bool			CanRefresh() const							{ return can_refresh_; }
};
