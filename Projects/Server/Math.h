#pragma once

#include <cmath>
#include <SFML/System/Vector2.hpp>

namespace Math
{
    template<typename T>
    concept Number = std::integral<T> || std::floating_point<T>;

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

    [[nodiscard]] inline size_t HashCombine(size_t first, size_t second) noexcept
    {
        return first ^ (second << 1);
    }
}

namespace std
{
    template<> struct hash<sf::Vector2i>
    {
        [[nodiscard]] size_t operator()(sf::Vector2i value) const noexcept
        {
            size_t first = std::hash<int>{}(value.x);
            size_t second = std::hash<int>{}(value.y);
            return Math::HashCombine(first, second);
        }
    };
}