#include "HermiteInterpolation.hpp"

HermiteInterpolation::HermiteInterpolation(const sf::Vector2f targetSize, const sf::Time duration)
{
    m_targetSize = targetSize;
    m_duration = duration;

    // Setup variables needed for the interpolation
    m_startPosition = m_targetSize / 2.f;
    m_endPosition = sf::Vector2f(random(0.f, m_targetSize.x), random(0.f, m_targetSize.y));
    m_nextPosition = sf::Vector2f(random(0.f, m_targetSize.x), random(0.f, m_targetSize.y));
    m_startTangent = sf::Vector2f(0.f, 0.f);
    m_endTangent = m_nextPosition - m_startPosition; // Catmull-Rom Spline (Faktor = 1)
}

sf::Vector2f HermiteInterpolation::nextStep(sf::Time elapsedTime)
{
    float amount = m_currentTimeClock.getElapsedTime().asSeconds() / m_duration.asSeconds();

    sf::Vector2f result = Hermite_Interpolation(m_startPosition, m_startTangent, m_endPosition, m_endTangent, amount);

    if(amount >= 1.f)
    {
        // interpolation has finished, reset variables
        m_startPosition = m_endPosition;

        m_startTangent = m_endTangent;
        m_endPosition = m_nextPosition;
        m_nextPosition = sf::Vector2f(random(0.f, m_targetSize.x), random(0.f, m_targetSize.y));
        m_endTangent = m_nextPosition - m_startPosition;

        m_currentTimeClock.restart();
    }
    return result;
}

void HermiteInterpolation::reset(sf::Vector2f lastPosition)
{
    m_startPosition = lastPosition;
    m_startTangent = sf::Vector2f(0.f, 0.f);
    m_endTangent = m_nextPosition - m_startPosition;

    m_currentTimeClock.restart();
}
