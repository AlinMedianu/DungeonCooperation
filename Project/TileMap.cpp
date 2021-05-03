#include "TileMap.h"

void TileMap::GetDrawn(Renderer& by)
{
	for (Tile& tile : tiles)
	{
		floor.SetPosition(tile.variation, tile.position);
		floor.GetDrawn(tile.variation, by);
	}
}
