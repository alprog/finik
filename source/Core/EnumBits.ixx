export module Finik.Core.EnumBits;

import std;

template <typename E>
concept Enum = std::is_enum_v<E>;

template<Enum E> 
constexpr auto to_underlying(E e) noexcept
{
    return static_cast<std::underlying_type_t<E>>(e);
}

export template<Enum E> 
constexpr E operator|(E lhs, E rhs) noexcept
{
    return static_cast<E>(to_underlying(lhs) | to_underlying(rhs));
}

export template<Enum E> 
constexpr E operator&(E lhs, E rhs) noexcept
{
    return static_cast<E>(to_underlying(lhs) & to_underlying(rhs));
}

export template<Enum E> 
constexpr E operator^(E lhs, E rhs) noexcept
{
    return static_cast<E>(to_underlying(lhs) ^ to_underlying(rhs));
}

export template<Enum E>
constexpr E& operator|=(E& lhs, E rhs) noexcept
{
    lhs = lhs | rhs;
    return lhs;
}

export template<Enum E> 
constexpr E& operator&=(E& lhs, E rhs) noexcept
{
    lhs = lhs & rhs;
    return lhs;
}

export template<Enum E> 
constexpr E& operator^=(E& lhs, E rhs) noexcept
{
    lhs = lhs ^ rhs;
    return lhs;
}

export template <Enum E>
constexpr E operator~(E e) noexcept
{
    return static_cast<E>(~to_underlying(e));
}
