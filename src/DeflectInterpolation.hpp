#ifndef DEFLECTINTERPOLATION_INCLUDE
#define DEFLECTINTERPOLATION_INCLUDE

#include "InterpolationMode.hpp"

class DeflectInterpolation : public InterpolationMode
{
public:
    DeflectInterpolation(const sf::Vector2f targetSize, const sf::Time duration = sf::seconds(0.6f));
    sf::Vector2f nextStep(const sf::Time elapsedTime);
    void reset(sf::Vector2f lastPosition);


private:
    sf::Vector2f m_targetSize;
    sf::Time m_duration;
    sf::Clock m_currentTimeClock;
    sf::Vector2f m_startPosition;
    sf::Vector2f m_endPosition;
    sf::Vector2f m_nextPosition;
    sf::Vector2f m_direction;
    sf::Vector2f m_startTangent;
    sf::Vector2f m_endTangent;
};


#endif // DEFLECTINTERPOLATION_INCLUDE

