#include "SmoothstepInterpolation.hpp"

SmoothstepInterpolation::SmoothstepInterpolation(const sf::Vector2f targetSize, const sf::Time duration)
{
    m_targetSize = targetSize;

    // Setup variables needed for the interpolation
    m_duration = duration;
    m_startPosition = m_targetSize / 2.f;
    m_endPosition = sf::Vector2f(random(0.f, m_targetSize.x), random(0.f, m_targetSize.y));
}

sf::Vector2f SmoothstepInterpolation::nextStep(const sf::Time elapsedTime)
{
    float amount = m_currentTimeClock.getElapsedTime().asSeconds() / m_duration.asSeconds();

    // amount cannot be greater that 1.0 otherwise Smoothstep_x2_Interpolation will return weird results
    amount = clamp(amount, 0.f, 1.f);

    sf::Vector2f result = Smoothstep_x2_Interpolation(m_startPosition, m_endPosition, amount);

    if(amount >= 1.f)
    {
        // interpolation has finished, reset variables
        m_startPosition = m_endPosition;

        m_endPosition = sf::Vector2f(random(0.f, m_targetSize.x), random(0.f, m_targetSize.y));

        m_currentTimeClock.restart();
    }
    return result;

}

void SmoothstepInterpolation::reset(sf::Vector2f lastPosition)
{
    m_startPosition = lastPosition;

    m_currentTimeClock.restart();
}

