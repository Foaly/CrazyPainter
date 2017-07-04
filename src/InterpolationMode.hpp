#ifndef INTERPOLATIONMODE_INCLUDE
#define INTERPOLATIONMODE_INCLUDE

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>

#include "Random.hpp"
#include "Utilities.hpp"

#ifndef PI
    #define PI 3.1415926535897932384626433832795f
#endif


namespace Interpolation
{
    enum InterpolationType
    {
        Hermite,
        Smoothstep,
        Jitter,
        Circle,
        Star,
        Deflect,

        InterpolationModeCount = 6
    };
}

inline Interpolation::InterpolationType& operator++(Interpolation::InterpolationType& obj)
{
    int i = obj;
    if( ++i >= Interpolation::InterpolationModeCount )
        i = Interpolation::Hermite;
    return obj = static_cast<Interpolation::InterpolationType>(i);
}

inline Interpolation::InterpolationType& operator--(Interpolation::InterpolationType& obj)
{
    int i = obj;
    if( --i <= -1 )
        i = Interpolation::InterpolationModeCount - 1;
    return obj = static_cast<Interpolation::InterpolationType>(i);
}


class InterpolationMode
{
public:
    virtual sf::Vector2f nextStep(const sf::Time elapsedTime) = 0;
    virtual void reset(sf::Vector2f lastPosition) = 0;
};


#endif // INTERPOLATIONMODE_INCLUDE
