#pragma once

#include "SpriteSheet.h"
#include "TileGeneration.h"

class TileMap
{
	Sprite::Sheet& floor/*, walls*/;
	std::vector<Tile> tiles;
public:
	template<TileGeneration::Stage... Stages>
	TileMap(Sprite::Sheet& floor, /*Sprite::Sheet& walls, */TileGeneration::Pipeline<Stages...> pipeline);
	void GetDrawn(Renderer& by);
};

template<TileGeneration::Stage... Stages>
TileMap::TileMap(Sprite::Sheet& floor, /*Sprite::Sheet& walls, */TileGeneration::Pipeline<Stages...> pipeline)
	:floor(floor), /*walls(walls), */tiles{}
{
	pipeline(tiles);
}
