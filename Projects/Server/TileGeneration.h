#pragma once

#include <cassert>
#include <array>
#include <unordered_map>
#include <tuple>
#include <SFML/Graphics/Rect.hpp>
#include "Tile.h"
#include "Math.h"
#include "RandomNumberGenerator.h"

namespace TileGeneration
{
    template<typename T>
    concept Stage = requires(T&& rule, std::unordered_map<sf::Vector2i, Tile>& tiles)
    {
        { rule(tiles) } -> std::same_as<void>;
    };

    template<Stage... Stages>
    class Pipeline
    {
        std::tuple<Stages...> stages;
    public:
        explicit Pipeline(Stages&&... stages) noexcept;
        void operator()(std::unordered_map<sf::Vector2i, Tile>& tiles) const;
    };

    template<Stage... Stages>
    Pipeline<Stages...>::Pipeline(Stages&&... stages) noexcept
        :stages(std::make_tuple(stages...))
    {
        
    }

    template<Stage... Stages>
    void Pipeline<Stages...>::operator()(std::unordered_map<sf::Vector2i, Tile>& tiles) const
    {
        std::apply([&tiles](Stages... stages)
            { 
                (stages(tiles), ...); 
            }, stages);
    }

    namespace Stages
    {
        class Rectangle
        {
            sf::IntRect bounds;
            Tile tile;
        public:
            explicit Rectangle(sf::IntRect&& bounds, Tile&& tile) noexcept;
            void operator()(std::unordered_map<sf::Vector2i, Tile>& tiles) const;
        };

        template<size_t count>
        class Except
        {
            std::array<sf::Vector2i, count> excluded;
        public:
            explicit Except(std::array<sf::Vector2i, count>&& excluded) noexcept;
            void operator()(std::unordered_map<sf::Vector2i, Tile>& tiles) const;
        };

        template<size_t count>
        Except<count>::Except(std::array<sf::Vector2i, count>&& excluded) noexcept
            :excluded(excluded)
        {

        }

        template<size_t count>
        void Except<count>::operator()(std::unordered_map<sf::Vector2i, Tile>& tiles) const
        {
            for (sf::Vector2i excludedTIle : excluded)
                tiles.erase(excludedTIle);
        }

        class RandomVariants
        {
            RandomNumberGenerator generator;
            size_t min, max;
        public:
            RandomVariants(size_t min, size_t max) noexcept;
            void operator()(std::unordered_map<sf::Vector2i, Tile>& tiles);
        };
    }

}

