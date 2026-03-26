#include "Components\HealthComponent.hpp"

#include "glm/common.hpp"

HealthComponent::HealthComponent() : 
health_(100), 
max_health_(100), 
wound_(0), 
vigor_(0)
{
	
}

void HealthComponent::Heal(const int healing, const int vigor, const bool revive)
{
	if (IsDead() && !revive)
	{
		return;
	}
	vigor_ += vigor;
	health_ = glm::clamp(health_ + healing, 0, max_health_ - wound_ + vigor_);
	
	if (on_healed_ != nullptr)
	{
		on_healed_(health_ ,healing, vigor_);
	}
}

void HealthComponent::Damage(const int damage, const int wound)
{
	if (IsDead())
	{
		return;
	}
	wound_ += glm::clamp(wound_ + wound, 0, max_health_ - 1);
	health_ = glm::clamp(health_ - damage, 0, max_health_ - wound_ + vigor_);
	if (on_damaged_ != nullptr)
	{
		on_damaged_(health_, damage, vigor_);
	}
	
	if (health_ <= 0)
	{
		Death();
	}
}

void HealthComponent::Restore()
{
	wound_ = 0;
	vigor_ = 0;
	health_ = max_health_;
}

void HealthComponent::IncreaseMaxHealth(const int amount, const bool healDifference)
{
	max_health_ += amount; 
	if (healDifference)
	{
		Heal(amount, 0);
	}
}

void HealthComponent::Death()
{
	if (on_death_ != nullptr)
	{
		on_death_();
	}
}
