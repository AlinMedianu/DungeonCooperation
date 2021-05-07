#pragma once

#include "SpriteSheet.h"
#include "TileGeneration.h"
#include "InputMover.h"

enum class YellowDungeonTIleset : size_t
{
	NorthWest, North, NorthEast, 
	West, Center, East, 
	SouthWest, South, SouthEast,
	COUNT
};

template<size_t tilesetsCount>
class TileMap
{
	std::array<Sprite::Sheet*, tilesetsCount> tilesets;
	std::unordered_map<sf::Vector2i, Tile> tiles;
public:
	template<TileGeneration::Stage... Stages>
	TileMap(std::array<Sprite::Sheet*, tilesetsCount>&& tilesets, TileGeneration::Pipeline<Stages...>&& pipeline);
	void Adjust(Input::Mover& mover) const;
	void GetDrawn(Renderer& by) const;
};

template<size_t tilesetsCount>
template<TileGeneration::Stage... Stages>
TileMap<tilesetsCount>::TileMap(std::array<Sprite::Sheet*, tilesetsCount>&& tilesets, TileGeneration::Pipeline<Stages...>&& pipeline)
	: tilesets(tilesets), tiles{}
{
	pipeline(tiles);
}

template<size_t tilesetsCount>
void TileMap<tilesetsCount>::Adjust(Input::Mover& mover) const
{
	auto moverPosition{ mover.GetPosition() };
	sf::Vector2i moverTileCoordinate{ sf::Vector2f{ std::round(moverPosition.x), std::round(moverPosition.y) } };
	if (tiles.at(moverTileCoordinate).blocked)
		mover.Move(mover.GetVelocity() * -1.f);
}

template<size_t tilesetsCount>
void TileMap<tilesetsCount>::GetDrawn(Renderer& by) const
{
	for (const auto& tile : tiles)
	{
		auto* tileset = tilesets[tile.second.tileSet];
		tileset->SetPosition(tile.first);
		tileset->ChangeTo(tile.second.variation);
		tileset->GetDrawn(by);
	}
}
