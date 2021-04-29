#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/View.hpp>
#include "SpriteAnimator.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Dungeon Cooperation", sf::Style::Titlebar | sf::Style::Close);
    Renderer renderer(window, 20.f);
    //sf::View camera({}, { 1280, 720 });
    //window.setView(camera);
    sf::Texture dungonTileset2{};
    dungonTileset2.loadFromFile(SpriteDirectory"0x72_DungeonTilesetII_v1.3.png");
    Sprite::Sheet playerBody(dungonTileset2, { 128, (1 * 2 + 1) * 32, 16, 32 }, 8, 1.f / 32);
    Sprite::Animator<Sprite::Animation::Player> player(playerBody, { 4, 4 }, 0.1f);
    sf::Clock frame;
    playerBody.Scale(2);
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
                else if (gameEvent.key.code == sf::Keyboard::Key::Num2)
                    player.Play(Sprite::Animation::Player::Walk);
                /*else if (gameEvent.key.code == sf::Keyboard::Key::W)
                {
                    camera.move(0, -100);
                    window.setView(camera);
                }*/
            }
        }
        window.clear();
        player.GetDrawn(renderer);
        window.display();
    }
    return EXIT_SUCCESS;
}