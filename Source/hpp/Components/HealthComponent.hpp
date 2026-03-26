#ifndef HEALTH_COMPONENT_HPP
#define HEALTH_COMPONENT_HPP
#pragma once

#include "BaseComponent.hpp"

#include <functional>

class HealthComponent : public BaseComponent
{
protected:
	int health_;
	int max_health_;
	int wound_;
	int vigor_;
	std::function<void(int, int, int)> on_healed_;
	std::function<void(int, int, int)> on_damaged_;
	std::function<void()> on_death_;
	
public:
	HealthComponent();
	~HealthComponent() override = default;
	
	void Heal(int healing, int vigor, bool revive = false);
	void Damage(int damage, int wound);
	void Restore();
	
	inline int GetHealth() const	{ return health_; }
	inline int GetMaxHealth() const { return max_health_; }
	inline int GetWound() const		{ return wound_; }
	inline int GetVigor() const		{ return vigor_; }
	
	inline bool IsDead() const		{ return health_ <= 0; }	
	
	void IncreaseMaxHealth(const int amount, const bool healDifference = true);
	
private:
	
	void Death();
	
	// TODO: Add health component attributes and functionality
	// TODO: Add enum for health component and initialize in Resource Manager
};

#endif