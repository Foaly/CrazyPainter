#ifndef SPIRALINTERPOLATION_INCLUDE
#define SPIRALINTERPOLATION_INCLUDE

#include "InterpolationMode.hpp"

class SpiralInterpolation : public InterpolationMode
{
public:
    SpiralInterpolation(const sf::Vector2f targetSize,
                        sf::Time duration = sf::Time::Zero,
                        sf::Time radiusDuration = sf::Time::Zero);

    sf::Vector2f nextStep(const sf::Time elapsedTime);
    void reset(sf::Vector2f lastPosition);


private:
    sf::Vector2f m_targetSize;
    const float m_maxRadius;
    sf::Time m_duration;
    sf::Clock m_currentTimeClock;
    sf::Vector2f m_origin;
    float m_oldRadius;
    float m_newRadius;
    sf::Time m_radiusDuration;
    sf::Clock m_currentRadiusTimeClock;
    sf::Time m_innerCircleDuration;
    sf::Clock m_innerCircleClock;
};


#endif // SPIRALINTERPOLATION_INCLUDE

