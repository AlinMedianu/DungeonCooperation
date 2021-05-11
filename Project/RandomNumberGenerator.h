#pragma once

#include <chrono>
#include <random>

class RandomNumberGenerator
{
	std::mt19937 engine;
public:
	RandomNumberGenerator() noexcept;
	int operator()(int min, int max);
};

