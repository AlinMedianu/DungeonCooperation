#pragma once

#include <cassert>
#include <vector>
#include <algorithm>
#include <SFML/System/Vector2.hpp>
#include "Math.h"
#include "Renderer.h"

namespace Sprite
{
	class Sheet
	{
		sf::Sprite body;
		std::vector<sf::IntRect> spriteCoordinates;
	public:
		explicit Sheet(const sf::Texture& source, const sf::Rect<unsigned>& firstSpriteLocation, 
			sf::Vector2u spriteCount, sf::Vector2u stride);
		[[nodiscard]] size_t Count() const noexcept;
		void ChangeTo(size_t spriteIndex);
		sf::Vector2f GetPosition() const;
		sf::FloatRect GetGlobalBounds() const;
		template<Math::Number Number>
		void SetPosition(sf::Vector2<Number> position);
		template<Math::Number Number>
		void SetPosition(Number x, Number y);
		void SetOrigin(float localX, float localY);
		template<Math::Number Number>
		void Move(sf::Vector2<Number> offset);
		template<Math::Number Number>
		void Move(Number offsetX, Number offsetY);
		void SetScale(float factor);
		void Scale(float factor);
		void Flip();
		void GetDrawn(Renderer& by) const;
	};

	template<Math::Number Number>
	inline void Sheet::SetPosition(sf::Vector2<Number> position)
	{
		body.setPosition(sf::Vector2f{ position });
	}

	template<Math::Number Number>
	inline void Sheet::SetPosition(Number x, Number y)
	{
		body.setPosition(x, y);
	}

	template<Math::Number Number>
	inline void Sheet::Move(sf::Vector2<Number> offset)
	{
		body.move(sf::Vector2f{ offset });
	}

	template<Math::Number Number>
	inline void Sheet::Move(Number offsetX, Number offsetY)
	{
		body.move(offsetX, offsetY);
	}
}
