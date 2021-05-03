#include <vector>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/View.hpp>
#include "SpriteAnimator.h"
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
    playerBody.SetScaleAll(1.f / 32);
    playerBody.SetOriginAll(0.5f, 1.f);
    playerBody.ScaleAll(2.5f);
    Sprite::Animator<Sprite::Animation::Player> playerAnimator(playerBody, { 4, 4 }, 0.1f);
    playerAnimator.Play(Sprite::Animation::Player::Idle);
    sf::Texture yellowBrickFloor{};
    yellowBrickFloor.loadFromFile(SpriteDirectory"Yellow Brick Floor.png");
    Sprite::Sheet floor(yellowBrickFloor, { 0, 0, 32, 32 }, { 4, 6 });
    floor.SetScaleAll(1.f / 32);
    floor.SetOriginAll(0.5f, 0.5f);
    TileGeneration::Pipeline pipeline
    (
        TileGeneration::Stages::Rectangle({ -5, 5, 10, 10 }),
        TileGeneration::Stages::RandomVariants(0, 23)
    );
    TileMap tiles(floor, pipeline);
    sf::Clock frame;
    while (window.isOpen())
    {
        float timeStep = frame.restart().asSeconds();
        sf::Event gameEvent;
        while (window.pollEvent(gameEvent))
        {
            switch (gameEvent.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyReleased:
                if (gameEvent.key.code == sf::Keyboard::Key::Num1)
                    playerAnimator.Play(Sprite::Animation::Player::Idle);
                else if (gameEvent.key.code == sf::Keyboard::Key::Num2)
                    playerAnimator.Play(Sprite::Animation::Player::Walk);
                /*else if (gameEvent.key.code == sf::Keyboard::Key::W)
                {
                    camera.move(0, -100);
                    window.setView(camera);
                }*/
            }
        }
        window.clear();
        tiles.GetDrawn(renderer);
        playerAnimator.GetDrawn(renderer);
        window.display();
    }
    return EXIT_SUCCESS;
}