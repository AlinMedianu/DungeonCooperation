#include "NetworkClient.h"
#include <SFML/Window/Event.hpp>
#include "SceneManager.h"
#include "MainMenuScene.h"
#include "TutorialScene.h"
using namespace std::chrono_literals;
int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Dungeon Cooperation", sf::Style::Titlebar | sf::Style::Close);
    std::this_thread::sleep_for(1s);
    Network::Client client(AddressAndPortLocation"Address and Port.txt");
    Renderer renderer(window, 40.f);
    Scenes::MainMenu mainMenu(renderer, window);
    Scenes::Tutorial tutorial;
    Scenes::Manager<Scenes::Type> sceneManager({ &mainMenu, &tutorial });
    while (window.isOpen())
    {
        client.Update();
        if (window.hasFocus())
        {
            sceneManager.UpdateCurrent();
            if (sceneManager.WantsToQuit())
            {
                window.close();
                break;
            }
        }
        sf::Event gameEvent;
        while (window.pollEvent(gameEvent))
        {
            switch (gameEvent.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            }
        }
        window.clear();
        sceneManager.DrawCurrent(renderer);
        window.display();
    }
    return EXIT_SUCCESS;
}