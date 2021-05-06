#include "TileMap.h"

void TileMap::GetDrawn(Renderer& by)
{
	for (Tile& tile : tiles)
	{
		floor.SetPosition(tile.position);
		floor.ChangeTo(tile.variation);
		floor.GetDrawn(by);
	}
}
