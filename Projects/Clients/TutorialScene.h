#pragma once

#include <SFML/Graphics/Texture.hpp>
#include "Scene.h"
#include "TileMap.h"

namespace Scenes
{
	class Tutorial : public Scene
	{
        sf::Texture dungonTileset2, yellowBrickFloor, yellowDungeonTileset;
        Sprite::Sheet playerBody, floor, walls;
        Sprite::Animator<Sprite::Animation::Player> playerAnimator;
        Input::Mover playerMover;
        TileMap<2> tutorialRoom;
        sf::Clock frame;
    public:
        explicit Tutorial();
        void Update() final;
        [[nodiscard]] Type WantsToTransitionTo() final;
        void GetDrawn(Renderer& renderer) const final;
	};
}