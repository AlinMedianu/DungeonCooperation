#pragma once

#include <cmath>
#include <SFML/System/Vector2.hpp>

namespace Math
{
    [[nodiscard]] inline float Length(sf::Vector2f vector)
    {
        return sqrtf(powf(vector.x, 2) + powf(vector.y, 2));
    }

    [[nodiscard]] inline void Normalize(sf::Vector2f& vector)
    {
        float magnitude = Length(vector);
        if (magnitude == 0)
            return;
        vector /= magnitude;
    }
}