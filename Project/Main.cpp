#include <SFML/Window/Event.hpp>
//#include <SFML/Graphics/View.hpp>
#include "SceneManager.h"
#include "MainMenuScene.h"
#include "TutorialScene.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Dungeon Cooperation", sf::Style::Titlebar | sf::Style::Close);
    Renderer renderer(window, 40.f);
    //sf::View camera({}, { 1280, 720 });
    //window.setView(camera);
    Scenes::MainMenu mainMenu(renderer, window);
    Scenes::Tutorial tutorial;
    Scenes::Manager<Scenes::Type> sceneManager({ &mainMenu, &tutorial });
    while (window.isOpen())
    {          
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
        sceneManager.DrawCurrent(renderer);
        window.display();
    }
    return EXIT_SUCCESS;
}