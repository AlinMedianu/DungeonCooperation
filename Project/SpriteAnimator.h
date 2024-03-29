#pragma once

#include <array>
#include <numeric>
#include "Enumeration.h"
#include "SpriteAnimation.h"

namespace Sprite
{
	enum class FacingDirection
	{
		Left, Right
	};

	template<Enumeration AnimationType>
	class Animator
	{
		std::vector<Animation> animations;
		AnimationType current;
	public:
		explicit Animator(Sheet& body, const std::array<size_t,
			static_cast<size_t>(AnimationType::COUNT)>& frameCountPerAnimation, float timeBetweenFrames);
		[[nodiscard]] constexpr AnimationType CurrentAnimation() const noexcept;
		void Play(AnimationType animation);
		void Stop() noexcept;
		void Update();
		void GetDrawn(Renderer& by) const;
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
		if (!animations[static_cast<size_t>(animation)].IsPlaying())
		{
			current = animation;
			Stop();
			animations[static_cast<size_t>(current)].Play();
		}
	}

	template<Enumeration AnimationType>
	void Animator<AnimationType>::Stop() noexcept
	{
		for (auto& animation : animations)
			animation.Stop();
	}

	template<Enumeration AnimationType>
	inline void Animator<AnimationType>::Update()
	{
		animations[static_cast<size_t>(current)].Update();
	}

	template<Enumeration AnimationType>
	inline void Animator<AnimationType>::GetDrawn(Renderer& by) const
	{
		animations[static_cast<size_t>(current)].GetDrawn(by);
	}
}