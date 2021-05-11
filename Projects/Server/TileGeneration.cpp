#include "TileGeneration.h"

namespace TileGeneration::Stages
{
	Rectangle::Rectangle(sf::IntRect&& bounds, Tile&& tile) noexcept
		:bounds(bounds), tile(tile)
	{
		assert(bounds.width >= 0 && bounds.height >= 0);
	}

	void Rectangle::operator()(std::unordered_map<sf::Vector2i, Tile>& tiles) const
	{
		tiles.reserve(tiles.size() + (bounds.width + 1) * (bounds.height + 1));
		int boundsBottom{ bounds.top - bounds.height }, boundsRight{ bounds.left + bounds.width };
		for (int coordinateY{ bounds.top }; coordinateY >= boundsBottom; --coordinateY)
			for (int courdinateX{ bounds.left }; courdinateX <= boundsRight; ++courdinateX)
				tiles.try_emplace(sf::Vector2i{ courdinateX, coordinateY }, tile);
	}

	RandomVariants::RandomVariants(size_t min, size_t max) noexcept
		: generator(), min(min), max(max)
	{

	}

	void RandomVariants::operator()(std::unordered_map<sf::Vector2i, Tile>& tiles)
	{
		for(auto& tile : tiles)
			tile.second.variation = generator(min, max);
	}
}