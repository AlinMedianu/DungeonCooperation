#include "SpriteSheet.h"

namespace Sprite
{
	Sheet::Sheet(const sf::Texture& source, const sf::Rect<unsigned>& firstSpriteLocation, sf::Vector2u spriteCount)
		: sprites{}
	{
		sprites.assign(spriteCount.x * spriteCount.y, sf::Sprite{ source });
		sf::IntRect nextSpriteLocation{ firstSpriteLocation };
		for (unsigned coordinateY{}; coordinateY < spriteCount.y; ++coordinateY)
		{
			for (unsigned coordinateX{}; coordinateX < spriteCount.x; ++coordinateX)
			{
				sprites[coordinateY * spriteCount.x + coordinateX].setTextureRect(nextSpriteLocation);
				nextSpriteLocation.left += nextSpriteLocation.width;
			}
			nextSpriteLocation.left = firstSpriteLocation.left;
			nextSpriteLocation.top += nextSpriteLocation.height;
		}
	}

	size_t Sheet::Count() const noexcept
	{
		return sprites.size();
	}

	void Sheet::SetPosition(size_t spriteIndex, sf::Vector2i position)
	{
		sprites[spriteIndex].setPosition(static_cast<float>(position.x), static_cast<float>(position.y));
	}

	void Sheet::SetPosition(size_t spriteIndex, float x, float y)
	{
		sprites[spriteIndex].setPosition(x, y);
	}

	void Sheet::SetOriginAll(float localX, float localY)
	{
		assert(localX >= 0 && localX <= 1 && localY >= 0 && localY <= 1);
		for (auto& sprite : sprites)
			sprite.setOrigin(sprite.getLocalBounds().left + sprite.getLocalBounds().width * localX,
				sprite.getLocalBounds().top + sprite.getLocalBounds().height * localY);
	}

	void Sheet::MoveAll(float offsetX, float offsetY)
	{
		for (auto& sprite : sprites)
			sprite.move(offsetX, offsetY);
	}

	void Sheet::SetScaleAll(float factor)
	{
		for (auto& sprite : sprites)
			sprite.setScale(factor, -factor);
	}

	void Sheet::ScaleAll(float factor)
	{
		assert(factor > 0);
		for (auto& sprite : sprites)
			sprite.scale(factor, factor);
	}

	void Sheet::FlipAll()
	{
		for (auto& sprite : sprites)
			sprite.scale(-1, 1);
	}

	void Sheet::GetDrawn(size_t spriteIndex, Renderer& by) const
	{
		assert(spriteIndex < sprites.size());
		by.Draw(sprites[spriteIndex]);
	}
}