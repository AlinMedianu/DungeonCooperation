#include "Renderer.h"

Renderer::Renderer(sf::RenderWindow& window, float worldUnitsPerScreenWidth)
	:window(window), worldToScreen{}
{
	float scaleFactor{ window.getSize().x / worldUnitsPerScreenWidth };
	worldToScreen.transform.scale(scaleFactor, -scaleFactor).
		translate(window.getSize().x / (2.f * scaleFactor), window.getSize().y / -(2.f * scaleFactor));
}

sf::FloatRect Renderer::WorldCoordinateToScreenPosition(sf::FloatRect worldCoordinates) const
{
	return worldToScreen.transform.transformRect(worldCoordinates);
}

sf::Vector2f Renderer::ScreenPositionToWorldCoordinate(sf::Vector2f screenPosition) const
{
	return worldToScreen.transform.getInverse().transformPoint(screenPosition);
}
