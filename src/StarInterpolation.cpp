#include "StarInterpolation.hpp"

StarInterpolation::StarInterpolation(const sf::Vector2f targetSize, const sf::Time duration)
{
    m_targetSize = targetSize;
    m_duration = duration;

    // Setup variables needed for the interpolation
    auto halfBorder = (targetSize.x - targetSize.y) / 2.f;
    m_bounds = sf::Vector2f(halfBorder, halfBorder + targetSize.y);
    m_startPosition = m_targetSize / 2.f;
    m_endPosition = sf::Vector2f(random(m_bounds.x, m_bounds.y), 0.f);
}

sf::Vector2f StarInterpolation::nextStep(const sf::Time elapsedTime)
{
    float amount = m_currentTimeClock.getElapsedTime().asSeconds() / m_duration.asSeconds();

    sf::Vector2f result = Linear_Interpolation(m_startPosition, m_endPosition, amount);

    if(amount >= 1.f)
    {
        // interpolation has finished, reset variables
        m_startPosition = m_endPosition;

        float y = 0.f;
        if (m_endPosition.y == 0.f)
            y = m_targetSize.y;

        m_endPosition = sf::Vector2f(random(m_bounds.x, m_bounds.y), y);

        m_currentTimeClock.restart();
    }
    return result;

}

void StarInterpolation::reset(sf::Vector2f lastPosition)
{
    m_startPosition = lastPosition;

    m_currentTimeClock.restart();
}

