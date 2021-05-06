#include "SpriteSheet.h"

namespace Sprite
{
	Sheet::Sheet(const sf::Texture& source, const sf::Rect<unsigned>& firstSpriteLocation, sf::Vector2u spriteCount)
		: body{ source }, spriteCoordinates{}
	{
		spriteCoordinates.assign(spriteCount.x * spriteCount.y, {});
		sf::IntRect nextSpriteLocation{ firstSpriteLocation };
		for (unsigned coordinateY{}; coordinateY < spriteCount.y; ++coordinateY)
		{
			for (unsigned coordinateX{}; coordinateX < spriteCount.x; ++coordinateX)
			{
				spriteCoordinates[coordinateY * spriteCount.x + coordinateX] = nextSpriteLocation;
				nextSpriteLocation.left += nextSpriteLocation.width;
			}
			nextSpriteLocation.left = firstSpriteLocation.left;
			nextSpriteLocation.top += nextSpriteLocation.height;
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

	void Sheet::SetPosition(sf::Vector2i position)
	{
		body.setPosition(static_cast<float>(position.x), static_cast<float>(position.y));
	}

	void Sheet::SetPosition(float x, float y)
	{
		body.setPosition(x, y);
	}

	void Sheet::SetOrigin(float localX, float localY)
	{
		assert(localX >= 0 && localX <= 1 && localY >= 0 && localY <= 1);
		body.setOrigin(body.getLocalBounds().left + body.getLocalBounds().width * localX,
			body.getLocalBounds().top + body.getLocalBounds().height * localY);
	}

	void Sheet::Move(sf::Vector2f offset)
	{
		body.move(offset);
	}

	void Sheet::Move(float offsetX, float offsetY)
	{
		body.move(offsetX, offsetY);
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