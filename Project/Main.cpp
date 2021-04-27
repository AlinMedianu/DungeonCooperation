#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "SpriteAnimator.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Dungeon Cooperation", sf::Style::Titlebar | sf::Style::Close);
    sf::Texture dungonTileset2{};
    dungonTileset2.loadFromFile(SpriteDirectory"0x72_DungeonTilesetII_v1.3.png");
    Sprite::Sheet playerBody(dungonTileset2, { 128, (1 * 2 + 1) * 32, 16, 32 }, 9);
    Sprite::Animator<Sprite::Animation::Player> player(playerBody, { 4, 4, 1 }, 0.1f);
    player.Move(300, 300);
    player.Scale(3);    
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
                    player.Play(Sprite::Animation::Player::Idle);
                if (gameEvent.key.code == sf::Keyboard::Key::Num2)
                    player.Play(Sprite::Animation::Player::Run);
                if (gameEvent.key.code == sf::Keyboard::Key::Num3)
                    player.Play(Sprite::Animation::Player::Hit);
            }
        }
        window.clear();
        player.GetDrawn(window);
        window.display();
    }

    return EXIT_SUCCESS;
}