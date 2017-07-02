#ifndef STARINTERPOLATION_INCLUDE
#define STARINTERPOLATION_INCLUDE

#include "InterpolationMode.hpp"

class StarInterpolation : public InterpolationMode
{
public:
    StarInterpolation(const sf::Vector2f targetSize, const sf::Time duration = sf::seconds(1.f));

    sf::Vector2f nextStep(const sf::Time elapsedTime);
    void reset(sf::Vector2f lastPosition);


private:
    sf::Vector2f m_targetSize;
    sf::Time m_duration;
    sf::Clock m_currentTimeClock;
    sf::Vector2f m_startPosition;
    sf::Vector2f m_endPosition;
    sf::Vector2f m_bounds;
};


#endif // STARINTERPOLATION_INCLUDE