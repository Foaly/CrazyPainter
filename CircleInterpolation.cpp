#include "CircleInterpolation.hpp"
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

CircleInterpolation::CircleInterpolation(const sf::Vector2f targetSize, const sf::Time duration, const sf::Time radiusDuration) :
    m_maxRadius(std::min(targetSize.x, targetSize.y) / 2.f)
{
    m_targetSize = targetSize;

    // Setup variables needed for the interpolation
    m_duration = duration;
    m_radiusDuration = radiusDuration;
    m_origin = m_targetSize / 2.f;
    m_oldRadius = random(0.f, m_maxRadius);
    m_newRadius = random(0.f, m_maxRadius);
}

sf::Vector2f CircleInterpolation::nextStep(const sf::Time elapsedTime)
{
    float amount = m_currentTimeClock.getElapsedTime().asSeconds() / m_duration.asSeconds();
    float angle = Linear_Interpolation(0.f, PI * 2, amount);

//    std::cout << angle << std::endl;
    if(amount >= 1.f)
    {
        // interpolation has finished, reset variables
        m_currentTimeClock.restart();
    }

    float radiusAmount = m_currentRadiusTimeClock.getElapsedTime().asSeconds() / m_radiusDuration.asSeconds();
    // amount cannot be greater that 1.0 otherwise Smoothstep_x2_Interpolation will return weird results
    radiusAmount = clamp(radiusAmount, 0.f, 1.f);
    float radius = Smoothstep_x2_Interpolation(m_oldRadius, m_newRadius, radiusAmount);

    if(radiusAmount >= 1.f)
    {
        // interpolation has finished, reset variables
        m_oldRadius = m_newRadius;

        float rand = random(0.f, m_maxRadius);
        //std::cout << rand << std::endl;
        m_newRadius = rand;

        m_currentRadiusTimeClock.restart();
    }


    return getPointOnCircle(m_origin, angle, radius);
}

void CircleInterpolation::reset(sf::Vector2f lastPosition)
{
    // set the old radius to the distance between the orgin and the last position
    m_oldRadius = std::sqrt(std::pow(lastPosition.x - m_origin.x, 2) + std::pow(lastPosition.y - m_origin.y, 2));

    m_currentTimeClock.restart();
    m_currentRadiusTimeClock.restart();
}
