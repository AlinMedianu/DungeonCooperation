#include "NetworkServer.h"
#include <SFML/Window/Event.hpp>
#include "SceneManager.h"
#include "MainMenuScene.h"
#include "TutorialScene.h"

int main()
{
    Network::Server server(AddressAndPortLocation"Address and Port.txt");
    while (!server.IsClosed())
    {
        server.Update();
        if (server.IsDisconnected())
            server.Close();
    }
    return EXIT_SUCCESS;
}