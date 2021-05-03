#pragma once

#include <cassert>
#include <array>
#include <vector>
#include <tuple>
#include <SFML/Graphics/Rect.hpp>
#include "Tile.h"
#include "RandomNumberGenerator.h"

namespace TileGeneration
{
    template<typename T>
    concept Stage = requires(T && rule, std::vector<Tile>& tiles)
    {
        { rule(tiles) } -> std::same_as<void>;
    };

    template<Stage... Stages>
    class Pipeline
    {
        std::tuple<Stages...> stages;
    public:
        Pipeline(Stages... stages) noexcept;
        void operator()(std::vector<Tile>& tiles);
    };

    template<Stage... Stages>
    Pipeline<Stages...>::Pipeline(Stages... stages) noexcept
        :stages(std::make_tuple(stages...))
    {
        
    }

    template<Stage... Stages>
    void Pipeline<Stages...>::operator()(std::vector<Tile>& tiles)
    {
        std::apply([&](Stages... stages){ (stages(tiles), ...); }, stages);
    }

    namespace Stages
    {
        class Rectangle
        {
            sf::IntRect bounds;
        public:
            explicit Rectangle(sf::IntRect&& bounds) noexcept;
            void operator()(std::vector<Tile>& tiles);
        };

        template<size_t count>
        class Except
        {
            std::array<sf::Vector2i, count> excluded;
        public:
            explicit Except(std::array<sf::Vector2i, count>&& excluded) noexcept;
            void operator()(std::vector<Tile>& tiles);
        };

        template<size_t count>
        Except<count>::Except(std::array<sf::Vector2i, count>&& excluded) noexcept
            :excluded(excluded)
        {

        }

        template<size_t count>
        void Except<count>::operator()(std::vector<Tile>& tiles)
        {
            std::erase_if(tiles, [&](Tile& tile)
                {
                    return std::find(excluded.begin(), excluded.end(), tile.position) != excluded.end();
                });
        }

        class RandomVariants
        {
            RandomNumberGenerator generator;
            size_t min, max;
        public:
            RandomVariants(size_t min, size_t max) noexcept;
            void operator()(std::vector<Tile>& tiles);
        };
    }

}

