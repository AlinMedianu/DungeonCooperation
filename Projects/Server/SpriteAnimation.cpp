#include "SpriteAnimation.h"

namespace Sprite
{
	Animation::Animation(Sheet& body, size_t firstFrame, size_t lastFrame, float timeBetweenFrames)
		: body(body), frameTimer{}, firstFrame{ firstFrame },
		currentFrame{ firstFrame }, lastFrame{ lastFrame }, timeBetweenFrames{ timeBetweenFrames }
	{
		assert(firstFrame >= 0 && firstFrame <= lastFrame && lastFrame < body.Count());
		body.ChangeTo(firstFrame);
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

	void Animation::Update()
	{
		if (IsPlaying() && frameTimer->getElapsedTime().asSeconds() >= timeBetweenFrames)
		{
			frameTimer->restart();
			if (++currentFrame > lastFrame)
				currentFrame = firstFrame;
			body.ChangeTo(currentFrame);
		}
	}

	void Animation::GetDrawn(Renderer& by) const
	{
		body.GetDrawn(by);
	}
}