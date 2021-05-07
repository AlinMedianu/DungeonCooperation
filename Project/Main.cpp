#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Texture.hpp>
//#include <SFML/Graphics/View.hpp>
#include "TileMap.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Dungeon Cooperation", sf::Style::Titlebar | sf::Style::Close);
    Renderer renderer(window, 40.f);
    //sf::View camera({}, { 1280, 720 });
    //window.setView(camera);
    sf::Texture dungonTileset2{};
    dungonTileset2.loadFromFile(SpriteDirectory"0x72_DungeonTilesetII_v1.3.png");   
    Sprite::Sheet playerBody(dungonTileset2, { 128, (1 * 2 + 1) * 32, 16, 32 }, { 8, 1 });
    playerBody.SetScale(1.f / 32);
    playerBody.SetOrigin(0.5f, 1.f);
    playerBody.Scale(2.5f);
    Sprite::Animator<Sprite::Animation::Player> playerAnimator(playerBody, { 4, 4 }, 0.1f);
    Input::Mover playerMover(playerBody, playerAnimator, 5.f, Sprite::FacingDirection::Right);
    sf::Texture yellowBrickFloor{};
    yellowBrickFloor.loadFromFile(SpriteDirectory"Yellow Brick Floor.png");
    Sprite::Sheet floor(yellowBrickFloor, { 0, 0, 32, 32 }, { 4, 6 });
    floor.SetScale(1.f / 32);
    floor.SetOrigin(0.5f, 0.5f);
    sf::Texture yellowDungeonTileset{};
    yellowDungeonTileset.loadFromFile(SpriteDirectory"Yellow Dungeon Tileset.png");
    Sprite::Sheet walls(yellowDungeonTileset, { 0, 0, 32, 32 }, { 3, 3 });
    walls.SetScale(1.f / 32);
    walls.SetOrigin(0.5f, 0.5f);
    TileGeneration::Pipeline pipeline
    (
        TileGeneration::Stages::Rectangle({ -5, 5, 10, 10 }, Tile{}),
        TileGeneration::Stages::RandomVariants(0, 23),
        TileGeneration::Stages::Rectangle({ -6, 6, 0, 0 }, 
            Tile
            { 
                .tileSet = 1,
                .variation = static_cast<size_t>(YellowDungeonTIleset::NorthWest),
                .blocked = true
            }),
        TileGeneration::Stages::Rectangle({ -5, 6, 10, 0 },
            Tile
            {
                .tileSet = 1,
                .variation = static_cast<size_t>(YellowDungeonTIleset::North),
                .blocked = true
            }),
        TileGeneration::Stages::Rectangle({ 6, 6, 0, 0 },
            Tile
            {
                .tileSet = 1,
                .variation = static_cast<size_t>(YellowDungeonTIleset::NorthEast),
                .blocked = true
            }),
        TileGeneration::Stages::Rectangle({ -6, 5, 0, 10 },
            Tile
            {
                .tileSet = 1,
                .variation = static_cast<size_t>(YellowDungeonTIleset::West),
                .blocked = true
            }),
        TileGeneration::Stages::Rectangle({ 6, 5, 0, 10 },
            Tile
            {
                .tileSet = 1,
                .variation = static_cast<size_t>(YellowDungeonTIleset::East),
                .blocked = true
            }),
        TileGeneration::Stages::Rectangle({ -6, -6, 0, 0 },
            Tile
            {
                .tileSet = 1,
                .variation = static_cast<size_t>(YellowDungeonTIleset::SouthWest),
                .blocked = true
            }),
        TileGeneration::Stages::Rectangle({ -5, -6, 10, 0 },
            Tile
            {
                .tileSet = 1,
                .variation = static_cast<size_t>(YellowDungeonTIleset::South),
                .blocked = true
            }),
        TileGeneration::Stages::Rectangle({ 6, -6, 0, 0 },
            Tile
            {
                .tileSet = 1,
                .variation = static_cast<size_t>(YellowDungeonTIleset::SouthEast),
                .blocked = true
            })
    );
    TileMap room(std::array<Sprite::Sheet*, 2>{ &floor, &walls }, std::move(pipeline));
    sf::Clock frame;
    while (window.isOpen())
    {
        float timeStep = frame.restart().asSeconds();
        playerMover.Update(timeStep);
        room.Adjust(playerMover);
        sf::Event gameEvent;
        while (window.pollEvent(gameEvent))
        { 
            switch (gameEvent.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyReleased:
                /*if (gameEvent.key.code == sf::Keyboard::Key::W)
                {
                    camera.move(0, -100);
                    window.setView(camera);
                }*/
                break;
            }
        }
        window.clear();
        room.GetDrawn(renderer);
        playerAnimator.GetDrawn(renderer);
        window.display();
    }
    return EXIT_SUCCESS;
}