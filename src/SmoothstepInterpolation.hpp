#ifndef SMOOTHSTEPINTERPOLATION_INCLUDE
#define SMOOTHSTEPINTERPOLATION_INCLUDE

#include "InterpolationMode.hpp"

class SmoothstepInterpolation : public InterpolationMode
{
public:
    SmoothstepInterpolation(const sf::Vector2f targetSize, const sf::Time duration = sf::seconds(1.f));
    sf::Vector2f nextStep(const sf::Time elapsedTime);
    void reset(sf::Vector2f lastPosition);


private:
    sf::Vector2f m_targetSize;
    sf::Time m_duration;
    sf::Clock m_currentTimeClock;
    sf::Vector2f m_startPosition;
    sf::Vector2f m_endPosition;
};


#endif // SMOOTHSTEPINTERPOLATION_INCLUDE
