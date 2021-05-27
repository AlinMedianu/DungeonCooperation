#pragma once

#include <array>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/Text.hpp>
#include "SpriteSheet.h"

namespace UI
{
	struct Button
	{		
		enum class State : size_t { Default, Hover, Down, Inactive, COUNT };
		explicit Button(Sprite::Sheet& body, sf::Text& message);
		void Update(const sf::Window& on);
		void Activate();
		void Deactivate();
		void Move(sf::Vector2f position);
		void SetScale(float factor);
		void SetOrigin(float localX, float localY);
		void RecalculateBounds(const Renderer& renderer);
		[[nodiscard]] bool WasClicked() noexcept;
		[[nodiscard]] State GetCurrentState() const noexcept;
		void GetDrawn(Renderer& by) const;
	private:
		Sprite::Sheet& body;
		sf::Text& message;
		sf::FloatRect bounds;
		State current;
		bool clicked;
	};
}