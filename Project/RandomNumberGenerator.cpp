#include "RandomNumberGenerator.h"

RandomNumberGenerator::RandomNumberGenerator() noexcept 
	: seed { static_cast<int>(std::chrono::system_clock::now().time_since_epoch().count()) }, engine(seed)
{

}

RandomNumberGenerator::RandomNumberGenerator(int seed) noexcept
	: seed{ seed }, engine(seed)
{

}

int RandomNumberGenerator::GetSeed() const noexcept
{
	return seed;
}

int RandomNumberGenerator::operator()(int min, int max)
{
	std::uniform_int_distribution distribution(min, max);
	return distribution(engine);
}