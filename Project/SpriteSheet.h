#pragma once

#include <cassert>
#include <vector>
#include <algorithm>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace Sprite
{
	class Sheet
	{
		std::vector<sf::Sprite> sprites;
	public:
		Sheet(const sf::Texture& source, const sf::IntRect& firstSpriteLocation, size_t spriteCount);
		[[nodiscard]] size_t Count() const noexcept;
		void Move(float offsetX, float offsetY);
		void Scale(float factor);
		void Flip();
		void GetDrawn(size_t spriteIndex, sf::RenderWindow& on) const;
	};
}
