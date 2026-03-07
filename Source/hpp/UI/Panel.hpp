#pragma once

#include "UI/Widget.hpp"

#include <map>
#include <vector>

class Panel
{
protected:
	friend class Game;
	std::map<unsigned int, std::vector<Widget*>> widgets_; // naturally maps are sorted by key on descending order
	bool active_;
	bool visible_;
	

public:
	
	Panel();
	virtual ~Panel();
	
	void AddWidget(const unsigned int layer, Widget& widget);
	
	inline void SetActive(const bool active) { active_ = active; }
	inline void SetVisible(const bool visible) { visible_ = visible; }
	
	inline bool IsActive() const { return active_; }
	inline bool IsVisible() const { return visible_; }
	
protected:
	void Start() const;
	void Update(const float deltaTime) const;
	void End() const;
};