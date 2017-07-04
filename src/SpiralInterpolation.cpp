#include "SpiralInterpolation.hpp"
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

SpiralInterpolation::SpiralInterpolation(const sf::Vector2f targetSize, sf::Time duration, sf::Time radiusDuration) :
    m_maxRadius(std::min(targetSize.x, targetSize.y) / 2.f)
{
    m_targetSize = targetSize;

    if (duration == sf::Time::Zero)
        duration = sf::seconds(10.f);
    if (radiusDuration == sf::Time::Zero)
        radiusDuration = sf::seconds(4.f);

    // Setup variables needed for the interpolation
    m_duration = duration;
    m_radiusDuration = radiusDuration;
    m_innerCircleDuration = sf::seconds(4.f);
    m_origin = m_targetSize / 2.f;
    m_oldRadius = random(0.f, m_maxRadius);
    m_newRadius = random(0.f, m_maxRadius);
}

sf::Vector2f SpiralInterpolation::nextStep(const sf::Time elapsedTime)
{
    float amount = m_currentTimeClock.getElapsedTime().asSeconds() / m_duration.asSeconds();
    amount = std::min(amount, 1.f);
    float angle = Linear_Interpolation(0.f, PI * 2, amount);

    if (amount >= 1.f)
    {
        // interpolation has finished, reset variables
        m_currentTimeClock.restart();
    }

    float radiusAmount = m_currentRadiusTimeClock.getElapsedTime().asSeconds() / m_radiusDuration.asSeconds();
    // amount cannot be greater that 1.0 otherwise Smoothstep_x2_Interpolation will return weird results
    radiusAmount = std::min(radiusAmount, 1.f);
    float radius = Smoothstep_x2_Interpolation(m_oldRadius, m_newRadius, radiusAmount);
    //float radius = 300.f;

    if (radiusAmount >= 1.f)
    {
        // interpolation has finished, reset variables
        m_oldRadius = m_newRadius;

        float rand = random(0.f, m_maxRadius);
        m_newRadius = rand;

        m_currentRadiusTimeClock.restart();
    }

    auto pointOnOuterCircle = getPointOnCircle(m_origin, angle, radius);

    float innerAmount = m_innerCircleClock.getElapsedTime().asSeconds() / m_innerCircleDuration.asSeconds();
    innerAmount = std::min(innerAmount, 1.f);
    float innerAngle = Linear_Interpolation(0.f, PI * 2, innerAmount);

    if (innerAmount >= 1.f)
        m_innerCircleClock.restart();

    return getPointOnCircle(pointOnOuterCircle, innerAngle, 150);
}

void SpiralInterpolation::reset(sf::Vector2f lastPosition)
{
    // set the old radius to the distance between the orgin and the last position
    m_oldRadius = std::sqrt(std::pow(lastPosition.x - m_origin.x, 2) + std::pow(lastPosition.y - m_origin.y, 2));

    m_currentTimeClock.restart();
    m_currentRadiusTimeClock.restart();
}
