#pragma once

#include <chrono>
#include <random>

class RandomNumberGenerator
{
	int seed;
	std::mt19937 engine;
public:
	RandomNumberGenerator() noexcept;
	explicit RandomNumberGenerator(int seed) noexcept;
	[[nodiscard]] int GetSeed() const noexcept;
	[[nodiscard]] int operator()(int min, int max);
};

