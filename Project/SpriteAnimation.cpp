#include "SpriteAnimation.h"

namespace Sprite
{
	Animation::Animation(Sheet& body, size_t firstFrame, size_t lastFrame, float timeBetweenFrames)
		: body(body), frameTimer{}, firstFrame{ firstFrame },
		currentFrame{ firstFrame }, lastFrame{ lastFrame }, timeBetweenFrames{ timeBetweenFrames }
	{
		assert(firstFrame >= 0 && firstFrame <= lastFrame && lastFrame < body.Count());
	}

	constexpr bool Animation::IsPlaying() const noexcept
	{
		return frameTimer.has_value();
	}

	void Animation::Play()
	{
		frameTimer.emplace();
	}

	void Animation::Stop() noexcept
	{
		frameTimer.reset();
	}

	void Animation::Move(float offsetX, float offsetY)
	{
		body.Move(offsetX, offsetY);
	}

	void Animation::Scale(float factor)
	{
		assert(factor > 0);
		body.Scale(factor);
	}

	void Animation::Flip()
	{
		body.Flip();
	}

	void Animation::GetDrawn(sf::RenderWindow& on)
	{
		body.GetDrawn(currentFrame, on);
		if (IsPlaying() && frameTimer->getElapsedTime().asSeconds() >= timeBetweenFrames)
		{
			frameTimer->restart();
			if (++currentFrame > lastFrame)
				currentFrame = firstFrame;
		}
	}
}