#include "SpriteSheet.h"

namespace Sprite
{
	Sheet::Sheet(const sf::Texture& source, const sf::IntRect& firstSpriteLocation, size_t spriteCount)
		: sprites{}
	{
		sprites.assign(spriteCount, sf::Sprite{ source });
		sf::IntRect nextSpriteLocation{ firstSpriteLocation };
		for (auto& sprite : sprites)
		{
			sprite.setTextureRect(nextSpriteLocation);
			nextSpriteLocation.left += nextSpriteLocation.width;
			sprite.setOrigin(sprite.getLocalBounds().left + sprite.getLocalBounds().width / 2.f,
				sprite.getLocalBounds().top + sprite.getLocalBounds().height / 2.f);
		}
	}

	size_t Sheet::Count() const noexcept
	{
		return sprites.size();
	}

	void Sheet::Move(float offsetX, float offsetY)
	{
		for (auto& sprite : sprites)
			sprite.move(offsetX, offsetY);
	}

	void Sheet::Scale(float factor)
	{
		assert(factor > 0);
		for (auto& sprite : sprites)
			sprite.setScale(factor, factor);
	}

	void Sheet::Flip()
	{
		for (auto& sprite : sprites)
		{
			auto scale{ sprite.getScale() };
			sprite.setScale(-scale.x, scale.y);
		}
	}

	void Sheet::GetDrawn(size_t spriteIndex, sf::RenderWindow& on) const
	{
		assert(spriteIndex < sprites.size());
		on.draw(sprites[spriteIndex]);
	}
}