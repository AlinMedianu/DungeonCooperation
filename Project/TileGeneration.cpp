#include "TileGeneration.h"

namespace TileGeneration::Stages
{
	Rectangle::Rectangle(sf::IntRect&& bounds) noexcept
		:bounds(bounds)
	{
		assert(bounds.width >= 0 && bounds.height >= 0);
	}

	void Rectangle::operator()(std::vector<Tile>& tiles)
	{
		tiles.reserve(tiles.size() + (bounds.width + 1) * (bounds.height + 1));
		int boundsBottom{ bounds.top - bounds.height }, boundsRight{ bounds.left + bounds.width };
		for (int coordinateY{ bounds.top }; coordinateY >= boundsBottom; --coordinateY)
			for (int courdinateX{ bounds.left }; courdinateX <= boundsRight; ++courdinateX)
				tiles.emplace_back(sf::Vector2i{ courdinateX, coordinateY }, 0);
	}

	RandomVariants::RandomVariants(size_t min, size_t max) noexcept
		: generator(), min(min), max(max)
	{

	}

	void RandomVariants::operator()(std::vector<Tile>& tiles)
	{
		for (Tile& tile : tiles)
			tile.variation = generator(min, max);
	}
}