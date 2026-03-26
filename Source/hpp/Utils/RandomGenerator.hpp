#ifndef RANDOMGENERATOR_HPP
#define RANDOMGENERATOR_HPP
#pragma once
#include <random>

class RandomGenerator
{
private:
	std::random_device device_;
	std::mt19937 generator_;
public:
	RandomGenerator();
	~RandomGenerator() = default;
	
	int GetRandomInt(int min, int max);
	float GetRandomFloat(float min, float max);
	double GetRandomDouble(double min, double max);
	bool GetRandomBool();
};

#endif
