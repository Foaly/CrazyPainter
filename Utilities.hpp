#ifndef UTILITIES_H
#define UTILITIES_H

#include <algorithm>
#include <SFML/System/Vector2.hpp>

template <typename T> T clamp(const T& value, const T& min, const T& max)
{
  return std::max(min, std::min(max, value));
}

template <typename T> T Linear_Interpolation(T& start, T& end, float& amount)
{
    return static_cast<T>((end * amount) + (start * (1 - amount)));
}

template <typename T> T Smoothstep_x2_Interpolation(T& start, T& end, float amount)
{
    // calculate smoothstep
    amount =  amount * amount * (3 - 2 * (amount));
    amount =  amount * amount * (3 - 2 * (amount));
    // calculate interpolation
    return (end * amount) + (start * (1 - amount));
}

template <typename T> T Hermite_Interpolation(const T& Value1, const T& Tangent1, const T& Value2, const T& Tangent2, const float amount)
{
    const T A(2.0f * Value1 - 2.0f * Value2 + Tangent1 + Tangent2);
    const T B(3.0f * Value2 - 3.0f * Value1 - 2.0f * Tangent1 - Tangent2);

    return A * (amount * amount * amount) + B * (amount * amount) + Tangent1 * (amount) + Value1;
}

inline sf::Vector2f getPointOnCircle(sf::Vector2f centerPoint, float angle, float radius)
{
    return sf::Vector2f(centerPoint.x + radius * std::cos(angle), centerPoint.y + radius * std::sin(angle));
}

#endif //UTILITIES_H
