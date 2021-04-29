#pragma once

#include <cassert>
#include <vector>
#include <algorithm>
#include "Renderer.h"

namespace Sprite
{
	class Sheet
	{
		std::vector<sf::Sprite> sprites;
	public:
		Sheet(const sf::Texture& source, const sf::IntRect& firstSpriteLocation, size_t spriteCount, float spriteWorldSize);
		[[nodiscard]] size_t Count() const noexcept;
		void Move(float offsetX, float offsetY);
		void Scale(float factor);
		void Flip();
		void GetDrawn(size_t spriteIndex, Renderer& on) const;
	};
}
