#ifndef SPRITE_ANIMATION_H
#define SPRITE_ANIMATION_H

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
	
public:
	SpriteAnimation();
	SpriteAnimation(const std::vector<std::string>& textureNames, const unsigned int framesPerSecond = 30);
	~SpriteAnimation();
	
	void Update(float deltaTime);
	void StartAnimation(const bool restart = true);
	void StopAnimation(const bool restart = true);
	
	void SetAnimationSprites(const std::vector<std::string>& names);
	inline void SetFramesPerSecond(const unsigned int framesPerSecond) { frames_per_second_ = framesPerSecond; }
	
	inline unsigned int GetFramesPerSecond() const { return frames_per_second_; }
	Texture2D GetCurrentFrameTexture() const; // Note: call after update to ensure the proper texture is provided this frame
};


#endif
