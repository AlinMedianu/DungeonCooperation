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
		sf::Sprite body;
		std::vector<sf::IntRect> spriteCoordinates;
	public:
		explicit Sheet(const sf::Texture& source, const sf::Rect<unsigned>& firstSpriteLocation, sf::Vector2u spriteCount);
		[[nodiscard]] size_t Count() const noexcept;
		void ChangeTo(size_t spriteIndex);
		sf::Vector2f GetPosition() const;
		void SetPosition(sf::Vector2i position);
		void SetPosition(float x, float y);
		void SetOrigin(float localX, float localY);
		void Move(sf::Vector2f offset);
		void Move(float offsetX, float offsetY);
		void SetScale(float factor);
		void Scale(float factor);
		void Flip();
		void GetDrawn(Renderer& by) const;
	};
}
