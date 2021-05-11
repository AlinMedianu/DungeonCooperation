#include "TutorialScene.h"

namespace Scenes
{
	Tutorial::Tutorial()
        : dungonTileset2{}, yellowBrickFloor{}, yellowDungeonTileset{},
        playerBody(dungonTileset2, { 128, (1 * 2 + 1) * 32, 16, 32 }, { 8, 1 }, {}),
        floor(yellowBrickFloor, { 0, 0, 32, 32 }, { 4, 6 }, {}),
        walls(yellowDungeonTileset, { 0, 0, 32, 32 }, { 3, 3 }, {}),
        playerAnimator(playerBody, { 4, 4 }, 0.1f), 
        playerMover(playerBody, playerAnimator, 5.f, Sprite::FacingDirection::Right),
        tutorialRoom(std::array<Sprite::Sheet*, 2>{ &floor, & walls }, TileGeneration::Pipeline
        (
            TileGeneration::Stages::Rectangle({ -5, 5, 10, 10 }, {}),
            TileGeneration::Stages::RandomVariants(0, 23),
            TileGeneration::Stages::Rectangle({ -6, 6, 0, 0 },
            {
                .tileSet = 1,
                .variation = static_cast<size_t>(YellowDungeonTIleset::NorthWest),
                .blocked = true
            }),
            TileGeneration::Stages::Rectangle({ -5, 6, 10, 0 },
            {
                .tileSet = 1,
                .variation = static_cast<size_t>(YellowDungeonTIleset::North),
                .blocked = true
            }),
            TileGeneration::Stages::Rectangle({ 6, 6, 0, 0 },
            {
                .tileSet = 1,
                .variation = static_cast<size_t>(YellowDungeonTIleset::NorthEast),
                .blocked = true
            }),
            TileGeneration::Stages::Rectangle({ -6, 5, 0, 10 },
            {
                .tileSet = 1,
                .variation = static_cast<size_t>(YellowDungeonTIleset::West),
                .blocked = true
            }),
            TileGeneration::Stages::Rectangle({ 6, 5, 0, 10 },
            {
                .tileSet = 1,
                .variation = static_cast<size_t>(YellowDungeonTIleset::East),
                .blocked = true
            }),
            TileGeneration::Stages::Rectangle({ -6, -6, 0, 0 },
            {
                .tileSet = 1,
                .variation = static_cast<size_t>(YellowDungeonTIleset::SouthWest),
                .blocked = true
            }),
            TileGeneration::Stages::Rectangle({ -5, -6, 10, 0 },
            {
                .tileSet = 1,
                .variation = static_cast<size_t>(YellowDungeonTIleset::South),
                .blocked = true
            }),
            TileGeneration::Stages::Rectangle({ 6, -6, 0, 0 },
            {
                .tileSet = 1,
                .variation = static_cast<size_t>(YellowDungeonTIleset::SouthEast),
                .blocked = true
            })
        )), frame{}
	{
        dungonTileset2.loadFromFile(SpriteDirectory"0x72_DungeonTilesetII_v1.3.png");
        playerBody.SetScale(1.f / 32);
        playerBody.SetOrigin(0.5f, 1.f);
        playerBody.Scale(2.5f);
        yellowBrickFloor.loadFromFile(SpriteDirectory"Yellow Brick Floor.png");
        floor.SetScale(1.f / 32);
        floor.SetOrigin(0.5f, 0.5f);
        yellowDungeonTileset.loadFromFile(SpriteDirectory"Yellow Dungeon Tileset.png");
        walls.SetScale(1.f / 32);
        walls.SetOrigin(0.5f, 0.5f);
	}

    void Tutorial::Update()
    {
        float timeStep = frame.restart().asSeconds();
        playerAnimator.Update();
        playerMover.Update(timeStep);
        tutorialRoom.Adjust(playerMover);
    }

    Type Tutorial::WantsToTransitionTo()
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            return Type::COUNT;
        return Type::Tutorial;
    }

    void Tutorial::GetDrawn(Renderer& renderer) const
    {
        tutorialRoom.GetDrawn(renderer);
        playerAnimator.GetDrawn(renderer);
    }
}