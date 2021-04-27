#pragma once

#include <optional>
#include <SFML/System/Clock.hpp>
#include "SpriteSheet.h"

namespace Sprite
{
	class Animation
	{
		Sheet& body;
		std::optional<sf::Clock> frameTimer;
		size_t firstFrame, currentFrame, lastFrame;
		const float timeBetweenFrames;
	public:
		enum class Player : size_t
		{
			Idle, Run, Hit,
			COUNT
		};
		Animation(Sheet& body, size_t firstFrame, size_t lastFrame, float timeBetweenFrames);
		[[nodiscard]] constexpr bool IsPlaying() const noexcept;
		void Play();
		void Stop() noexcept;
		void Move(float offsetX, float offsetY);
		void Scale(float factor);
		void Flip();
		void GetDrawn(sf::RenderWindow& on);
	};
}