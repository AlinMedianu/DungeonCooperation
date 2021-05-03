#pragma once

#include <cassert>
#include <vector>
#include <algorithm>
#include <SFML/System/Vector2.hpp>
#include "Renderer.h"

namespace Sprite
{
	class Sheet
	{
		std::vector<sf::Sprite> sprites;
	public:
		explicit Sheet(const sf::Texture& source, const sf::Rect<unsigned>& firstSpriteLocation, sf::Vector2u spriteCount);
		[[nodiscard]] size_t Count() const noexcept;
		void SetPosition(size_t spriteIndex, sf::Vector2i position);
		void SetPosition(size_t spriteIndex, float x, float y);
		void SetOriginAll(float localX, float localY);
		void MoveAll(float offsetX, float offsetY);
		void SetScaleAll(float factor);
		void ScaleAll(float factor);
		void FlipAll();
		void GetDrawn(size_t spriteIndex, Renderer& by) const;
	};
}
