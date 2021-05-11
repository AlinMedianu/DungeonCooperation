#include "MainMenuScene.h"

namespace Scenes
{
	MainMenu::MainMenu(Renderer& renderer, sf::Window& window)
        : buttonTexture{}, buttonBodies
    { 
        Sprite::Sheet(buttonTexture, { 178, 201, 363, 179 },
            { static_cast<size_t>(UI::Button::State::COUNT), 1 }, { 22,0 }),
        buttonBodies[static_cast<size_t>(Buttons::Singleplayer)], 
        buttonBodies[static_cast<size_t>(Buttons::Singleplayer)], 
        buttonBodies[static_cast<size_t>(Buttons::Singleplayer)] 
    }, arial{},
        buttonsText
    { 
        sf::Text("Singleplayer", arial), sf::Text("Multiplayer", arial),
        sf::Text("Options", arial), sf::Text("Quit", arial) 
    },
        buttons
    { 
        UI::Button(buttonBodies[static_cast<size_t>(Buttons::Singleplayer)], 
        buttonsText[static_cast<size_t>(Buttons::Singleplayer)]),
        UI::Button(buttonBodies[static_cast<size_t>(Buttons::Multiplayer)], 
        buttonsText[static_cast<size_t>(Buttons::Multiplayer)]),
        UI::Button(buttonBodies[static_cast<size_t>(Buttons::Options)], 
        buttonsText[static_cast<size_t>(Buttons::Options)]),
        UI::Button(buttonBodies[static_cast<size_t>(Buttons::Quit)], 
        buttonsText[static_cast<size_t>(Buttons::Quit)])
    }, window(window)

	{
        buttonTexture.loadFromFile(SpriteDirectory"Button.png");
        arial.loadFromFile(FontDirectory"arial.ttf");
        for (auto& buttonText : buttonsText)
            buttonText.setFillColor(sf::Color::Black);
        for (auto& button : buttons)
        {
            button.SetScale(1.f / 64);
            button.SetOrigin(0.5f, 0.5f);
        }
        auto& singleplayerButton(buttons[static_cast<size_t>(Buttons::Singleplayer)]);
        singleplayerButton.Move({ 0, 3 });
        auto& optionsButton(buttons[static_cast<size_t>(Buttons::Options)]);
        optionsButton.Move({ 0, -3 });
        buttons[static_cast<size_t>(Buttons::Quit)].Move({ 0, -6 });
        for (auto& button : buttons)
            button.RecalculateBounds(renderer);
        singleplayerButton.Deactivate();
        optionsButton.Deactivate();
	}

    void MainMenu::Update()
    {
        for (auto& button : buttons)
            button.Update(window);
    }

    Type MainMenu::WantsToTransitionTo()
    {
        if (buttons[static_cast<size_t>(Buttons::Multiplayer)].WasClicked())
            return Type::Tutorial;
        if (buttons[static_cast<size_t>(Buttons::Quit)].WasClicked())
            return Type::COUNT;
        return Type::MainMenu;
    }

    void MainMenu::GetDrawn(Renderer& renderer) const
    {
        for (auto& button : buttons)
            button.GetDrawn(renderer);
    }
}