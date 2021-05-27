#pragma once

#include <type_traits>

template<typename T>
concept Enumeration = std::is_enum_v<T> && requires()
{
    { T::COUNT } -> std::same_as<T>;
};

template<typename T>
concept BitMask = std::is_enum_v<T> && requires()
{
    { T::None } -> std::same_as<T>;
    { T::All } -> std::same_as<T>;
};

template<BitMask Mask>
constexpr inline Mask operator~ (Mask mask)
{ 
    return static_cast<Mask>(~static_cast<std::underlying_type_t<Mask>>(mask));
}

template<BitMask Mask>
constexpr inline Mask operator| (Mask first, Mask second)
{ 
    return static_cast<Mask>(static_cast<std::underlying_type_t<Mask>>(first) |
        static_cast<std::underlying_type_t<Mask>>(second));
}

template<BitMask Mask>
constexpr inline Mask operator& (Mask first, Mask second)
{ 
    return static_cast<Mask>(static_cast<std::underlying_type_t<Mask>>(first) &
        static_cast<std::underlying_type_t<Mask>>(second));
}

template<BitMask Mask>
constexpr void operator|= (Mask first, Mask second)
{ 
    first = first | second;
}

template<BitMask Mask>
constexpr void operator&= (Mask first, Mask second)
{ 
    first = first & second;
}