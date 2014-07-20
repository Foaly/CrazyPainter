#ifndef CIRCLEINTERPOLATION_INCLUDE
#define CIRCLEINTERPOLATION_INCLUDE

#include "InterpolationMode.hpp"

class CircleInterpolation : public InterpolationMode
{
public:
    CircleInterpolation(const sf::Vector2f targetSize, const sf::Time duration = sf::seconds(4.f), const sf::Time radiusDuration = sf::seconds(1.0));
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
};


#endif // CIRCLEINTERPOLATION_INCLUDE

