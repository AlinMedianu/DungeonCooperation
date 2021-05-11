#include "RandomNumberGenerator.h"

RandomNumberGenerator::RandomNumberGenerator() noexcept :
	engine((int)std::chrono::system_clock::now().time_since_epoch().count())
{

}

int RandomNumberGenerator::operator()(int min, int max)
{
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(engine);
}