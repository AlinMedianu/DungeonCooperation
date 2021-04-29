#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Sprite.hpp>

template<typename T>
concept Drawable = std::derived_from<T, sf::Drawable> && std::derived_from<T, sf::Transformable>;

class Renderer
{
	sf::RenderWindow& window;
	sf::RenderStates worldToScreen;
public:
	Renderer(sf::RenderWindow& window, float worldUnitsPerScreenWidth);
	void Draw(const Drawable auto& drawable);
};

inline void Renderer::Draw(const Drawable auto& drawable)
{
	window.draw(drawable, worldToScreen);
}
