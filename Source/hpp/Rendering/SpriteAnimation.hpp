#ifndef SPRITE_ANIMATION_HPP
#define SPRITE_ANIMATION_HPP
#pragma once

#include "Texture2D.hpp"

#include <string>
#include <vector>

class SpriteAnimation
{
private:
	std::vector<Texture2D> textures_;
	float current_time_;
	unsigned int current_frame_;
	unsigned int frames_per_second_;
	bool is_playing_;
	bool play_once_;
	
	//TODO: We might want to add delegates, also we might want to add other flags to control how we play the animation
	
public:
	SpriteAnimation();
	explicit SpriteAnimation(const std::vector<std::string>& textureNames, const unsigned int framesPerSecond = 30);
	~SpriteAnimation();
	
	void Update(const float deltaTime);
	void StartAnimation(const bool playOnce = false, const bool restart = true);
	void StopAnimation(const bool restart = true);
	
	// Setters
	void		SetAnimationSprites(const std::vector<std::string>& names);
	inline void SetFramesPerSecond(const unsigned int framesPerSecond) { frames_per_second_ = framesPerSecond; }
	
	// Getters
	inline unsigned int GetFramesPerSecond() const { return frames_per_second_; }
	Texture2D			GetCurrentFrameTexture() const; // Note: call after update to ensure the proper texture is provided this frame
	inline bool			IsPlaying() const { return is_playing_; }
	
private:
	inline void Restart() 
	{
		current_frame_ = 0;
		current_time_ = 0.0f;
	};
};
#endif
