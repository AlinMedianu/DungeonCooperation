#pragma once

#include <type_traits>

template<typename T>
concept Enumeration = std::is_enum_v<T> && requires()
{
    { T::COUNT };
};