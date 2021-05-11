#pragma once

#include <SFML/System/Vector2.hpp>

struct Tile
{
	size_t tileSet;
	size_t variation;
	bool blocked;
};