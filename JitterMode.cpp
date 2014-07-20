#include "JitterMode.hpp"

JitterMode::JitterMode(const sf::Vector2f targetSize)
{
    m_targetSize = targetSize;

    // Setup variables needed for the interpolation
    m_oldPosition = m_targetSize / 2.f;
}

sf::Vector2f JitterMode::nextStep(const sf::Time elapsedTime)
{
    sf::Vector2f result = sf::Vector2f(m_oldPosition.x + random(-10.f, 10.f), m_oldPosition.y + random(-10.f, 10.f));
    m_oldPosition = result;

    // if the points are outside the target, reset them to the center
    if(result.x < 0 || result.x > m_targetSize.x || result.y < 0 || result.y > m_targetSize.y)
    {
        result = sf::Vector2f(-1.f, -1.f);
    }
    return result;

}

void JitterMode::reset(sf::Vector2f lastPosition)
{
    m_oldPosition = lastPosition;
}


