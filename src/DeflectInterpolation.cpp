#include "DeflectInterpolation.hpp"
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

DeflectInterpolation::DeflectInterpolation(const sf::Vector2f targetSize, const sf::Time duration)
{
    m_targetSize = targetSize;

    // Setup variables needed for the interpolation
    m_duration = duration;
    m_startPosition = m_targetSize / 2.f;
    m_direction = sf::Vector2f(0.f, 150.f);
    m_endPosition = m_startPosition + m_direction;
    m_nextPosition = m_endPosition + m_direction;
    m_startTangent = sf::Vector2f(0.f, 0.f);
    m_endTangent = m_startPosition - m_nextPosition;
}

sf::Vector2f DeflectInterpolation::nextStep(const sf::Time elapsedTime)
{
    float amount = m_currentTimeClock.getElapsedTime().asSeconds() / m_duration.asSeconds();
    amount = std::min(amount, 1.f);

    sf::Vector2f result = Hermite_Interpolation(m_startPosition, m_startTangent, m_endPosition, m_endTangent, amount);

    if(amount >= 1.f)
    {
        // interpolation has finished, reset variables
        m_startPosition = m_endPosition;
        m_startTangent = m_endTangent;
        m_endPosition = m_nextPosition;

        // deflect the current direction by 30°
        //float angle = randomDev(0.f, 30.f);
        float angle = random(0.f, 360.f);
        float cos = std::cos(angle);
        float sin = std::sin(angle);

        m_direction = sf::Vector2f(cos * m_direction.x - sin * m_direction.y,
                                   sin * m_direction.x + cos * m_direction.y);

        auto next = m_endPosition + m_direction;

        if (next.x > m_targetSize.x || next.x < 0.f)
            m_direction.x *= -1.f;
        if (next.y > m_targetSize.y || next.y < 0.f)
            m_direction.y *= -1.f;

        m_nextPosition = m_endPosition + m_direction;
        m_endTangent = m_startPosition - m_nextPosition;

        m_currentTimeClock.restart();
    }
    return result;
}

void DeflectInterpolation::reset(sf::Vector2f lastPosition)
{
    // TODO: ensure addition stays inbounds (make direction point to center)
    m_startPosition = lastPosition;
    m_endPosition = m_startPosition + m_direction;
    m_nextPosition = m_endPosition + m_direction;
    m_startTangent = sf::Vector2f(0.f, 0.f);
    m_endTangent = m_startPosition - m_nextPosition;

    m_currentTimeClock.restart();
}
