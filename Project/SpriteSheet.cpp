#include "SpriteSheet.h"

namespace Sprite
{
	Sheet::Sheet(const sf::Texture& source, const sf::Rect<unsigned>& firstSpriteLocation, 
		sf::Vector2u spriteCount, sf::Vector2u stride)
		: body{ source }, spriteCoordinates{}
	{
		spriteCoordinates.assign(spriteCount.x * spriteCount.y, {});
		sf::IntRect nextSpriteLocation{ firstSpriteLocation };
		for (unsigned coordinateY{}; coordinateY < spriteCount.y; ++coordinateY)
		{
			for (unsigned coordinateX{}; coordinateX < spriteCount.x; ++coordinateX)
			{
				spriteCoordinates[coordinateY * spriteCount.x + coordinateX] = nextSpriteLocation;
				nextSpriteLocation.left += nextSpriteLocation.width + stride.x;
			}
			nextSpriteLocation.left = firstSpriteLocation.left + stride.x;
			nextSpriteLocation.top += nextSpriteLocation.height + stride.y;
		}
		body.setTextureRect(spriteCoordinates[0]);
	}

	size_t Sheet::Count() const noexcept
	{
		return spriteCoordinates.size();
	}

	void Sheet::ChangeTo(size_t spriteIndex)
	{
		body.setTextureRect(spriteCoordinates[spriteIndex]);
	}

	sf::Vector2f Sheet::GetPosition() const
	{
		return body.getPosition();
	}

	sf::FloatRect Sheet::GetGlobalBounds() const
	{
		return body.getGlobalBounds();
	}

	void Sheet::SetOrigin(float localX, float localY)
	{
		assert(localX >= 0 && localX <= 1 && localY >= 0 && localY <= 1);
		body.setOrigin(body.getLocalBounds().left + body.getLocalBounds().width * localX,
			body.getLocalBounds().top + body.getLocalBounds().height * localY);
	}

	void Sheet::SetScale(float factor)
	{
		body.setScale(factor, -factor);
	}

	void Sheet::Scale(float factor)
	{
		assert(factor > 0);
		body.scale(factor, factor);
	}

	void Sheet::Flip()
	{
		body.scale(-1, 1);
	}

	void Sheet::GetDrawn(Renderer& by) const
	{
		by.Draw(body);
	}
}