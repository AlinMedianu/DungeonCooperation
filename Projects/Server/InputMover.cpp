#include "InputMover.h"

namespace Input
{
	Mover::Mover(Sprite::Sheet& body, Sprite::Animator<Sprite::Animation::Player>& animator, 
		float speed, Sprite::FacingDirection startingDirection) noexcept
		:body(body), animator(animator), velocity{}, speed(speed), facingDirection(startingDirection)
	{
		animator.Play(Sprite::Animation::Player::Idle);
	}

	sf::Vector2f Mover::GetPosition() const
	{
		return body.GetPosition();
	}

	sf::Vector2f Mover::GetVelocity() const
	{
		return velocity;
	}

	void Mover::Move(sf::Vector2f amount)
	{
		body.Move(amount);
	}

	void Mover::Look(Sprite::FacingDirection direction)
	{
		if (direction != facingDirection)
		{
			body.Flip();
			facingDirection = direction;
		}
	}

	void Mover::Update(float timeStep)
	{
		velocity = {};
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			velocity += { 0, 1 };
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			velocity += { -1, 0 };
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			velocity += { 0, -1 };
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			velocity += { 1, 0 };
		Math::Normalize(velocity);
		if (velocity != sf::Vector2f{})
		{
			velocity *= speed * timeStep;
			body.Move(velocity);
			animator.Play(Sprite::Animation::Player::Walk);
			if (velocity.x > 0)
				Look(Sprite::FacingDirection::Right);
			else if (velocity.x < 0)
				Look(Sprite::FacingDirection::Left);
		}
		else
			animator.Play(Sprite::Animation::Player::Idle);
	}
}
