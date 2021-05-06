#pragma once

#include <SFML/Window/Keyboard.hpp>
#include "Math.h"
#include "SpriteAnimator.h"

namespace Input
{
	class Mover
	{
		Sprite::Sheet& body;
		Sprite::Animator<Sprite::Animation::Player>& animator;
		sf::Vector2f velocity;
		float speed;
		Sprite::FacingDirection facingDirection;
	public:
		explicit Mover(Sprite::Sheet& body, Sprite::Animator<Sprite::Animation::Player>& animator, 
			float speed, Sprite::FacingDirection startingDirection) noexcept;
		sf::Vector2f GetVelocity() const;
		void Look(Sprite::FacingDirection direction);
		void Update(float timeStep);
	};
}
