#ifndef JITTER_INCLUDE
#define JITTER_INCLUDE

#include "InterpolationMode.hpp"

class JitterMode : public InterpolationMode
{
public:
    JitterMode(const sf::Vector2f targetSize);
    sf::Vector2f nextStep(const sf::Time elapsedTime);
    void reset(sf::Vector2f lastPosition);

private:
    sf::Vector2f m_targetSize;
    sf::Vector2f m_oldPosition;
};

#endif // JITTER_INCLUDE
