#pragma once

#include "Scene.h"
#include "UIButton.h"

namespace Scenes
{
	class MainMenu : public Scene
	{
        enum class Buttons
        {
            Singleplayer, Multiplayer, Options, Quit,
            COUNT
        };
        sf::Texture buttonTexture;
        std::array<Sprite::Sheet, static_cast<size_t>(Buttons::COUNT)> buttonBodies;
        sf::Font arial;
        std::array<sf::Text, static_cast<size_t>(Buttons::COUNT)> buttonsText;
        std::array<UI::Button, static_cast<size_t>(Buttons::COUNT)> buttons;
        sf::Window& window;
    public:
        explicit MainMenu(Renderer& renderer, sf::Window& window);
        void Update() final;
        [[nodiscard]] Type WantsToTransitionTo() final;
        void GetDrawn(Renderer& renderer) const final;
	};
}