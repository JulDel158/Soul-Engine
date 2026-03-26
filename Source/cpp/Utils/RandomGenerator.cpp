#include "Utils/RandomGenerator.hpp"

RandomGenerator::RandomGenerator() :
generator_(device_())
{
}

int RandomGenerator::GetRandomInt(const int min, const int max)
{
	std::uniform_int_distribution<> distribution(min, max);
	return distribution(generator_);
}

float RandomGenerator::GetRandomFloat(const float min, const float max)
{
	std::uniform_real_distribution<> distribution(min, max);
	return static_cast<float>(distribution(generator_));
}

double RandomGenerator::GetRandomDouble(double min, double max)
{
	std::uniform_real_distribution<> distribution(min, max);
	return distribution(generator_);
}

bool RandomGenerator::GetRandomBool()
{
	std::uniform_int_distribution<> distribution(0, 1);
	return static_cast<bool>(distribution(generator_));
}
