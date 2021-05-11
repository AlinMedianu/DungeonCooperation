#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "Math.h"

template<typename T>
concept Drawable = std::derived_from<T, sf::Drawable> && std::derived_from<T, sf::Transformable>;

class Renderer
{
	sf::RenderWindow& window;
	sf::RenderStates worldToScreen;
public:
	explicit Renderer(sf::RenderWindow& window, float worldUnitsPerScreenWidth);
	sf::FloatRect WorldCoordinateToScreenPosition(sf::FloatRect worldCoordinates) const;
	sf::Vector2f ScreenPositionToWorldCoordinate(sf::Vector2f screenPosition) const;
	void Draw(const Drawable auto& drawable);
};

inline void Renderer::Draw(const Drawable auto& drawable)
{
	window.draw(drawable, worldToScreen);
}
