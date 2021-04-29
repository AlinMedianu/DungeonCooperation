#pragma once

#include <array>
#include <numeric>
#include "Enumeration.h"
#include "SpriteAnimation.h"

namespace Sprite
{
	template<Enumeration AnimationType>
	class Animator
	{
		std::vector<Animation> animations;
		AnimationType current;
	public:
		Animator(Sheet& body, const std::array<size_t,
			static_cast<size_t>(AnimationType::COUNT)>& frameCountPerAnimation, float timeBetweenFrames);
		[[nodiscard]] constexpr AnimationType CurrentAnimation() const noexcept;
		void Play(AnimationType animation);
		void Stop() noexcept;
		void Move(float offsetX, float offsetY);
		void Scale(float factor);
		void Flip();
		void GetDrawn(Renderer& on);
	};

	template<Enumeration AnimationType>
	Animator<AnimationType>::Animator(Sheet& body, const std::array<size_t, 
		static_cast<size_t>(AnimationType::COUNT)>& frameCountPerAnimation, float timeBetweenFrames)
		: animations{}, current{}
	{
		assert(std::accumulate(frameCountPerAnimation.begin(), frameCountPerAnimation.end(), 0) == body.Count());
		std::transform(frameCountPerAnimation.begin(), frameCountPerAnimation.end(), std::back_inserter(animations),
			[&body, firstFrame = 0, timeBetweenFrames](size_t frameCount) mutable
		{
			size_t lastFrame = firstFrame + frameCount - 1;
			Sprite::Animation animation(body, firstFrame, lastFrame, timeBetweenFrames);
			firstFrame = ++lastFrame;
			return animation;
		});
	}

	template<Enumeration AnimationType>
	inline constexpr AnimationType Animator<AnimationType>::CurrentAnimation() const noexcept
	{
		return current;
	}

	template<Enumeration AnimationType>
	void Animator<AnimationType>::Play(AnimationType animation)
	{
		current = animation;
		Stop();
		animations[static_cast<size_t>(current)].Play();
	}

	template<Enumeration AnimationType>
	void Animator<AnimationType>::Stop() noexcept
	{
		for (auto& animation : animations)
			animation.Stop();
	}

	template<Enumeration AnimationType>
	inline void Animator<AnimationType>::Move(float offsetX, float offsetY)
	{
		animations[static_cast<size_t>(current)].Move(offsetX, offsetY);
	}

	template<Enumeration AnimationType>
	inline void Animator<AnimationType>::Scale(float factor)
	{
		animations[static_cast<size_t>(current)].Scale(factor);
	}

	template<Enumeration AnimationType>
	inline void Animator<AnimationType>::Flip()
	{
		animations[static_cast<size_t>(current)].Flip();
	}

	template<Enumeration AnimationType>
	inline void Animator<AnimationType>::GetDrawn(Renderer& on)
	{
		animations[static_cast<size_t>(current)].GetDrawn(on);
	}
}