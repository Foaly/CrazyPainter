#ifndef COLORGENERATOR_INCLUDE
#define COLORGENERATOR_INCLUDE

#include <SFML\System\Time.hpp>
#include <SFML\Graphics\Color.hpp>
#include "Utilities.hpp"

class ColorGenerator
{
public:
	ColorGenerator();
    sf::Color generateInterpolatedColor(sf::Time deltaTime);
    sf::Color generateHueShiftedColor(float hueStep);
    static sf::Color getRandomColor();
    static sf::Color getRandomHue();
    static sf::Color getRandomColorLessGray();

private:
    static sf::Color hueToRGB(float hue);


    sf::Color m_startColor;
    sf::Color m_endColor;
    sf::Time m_duration;
    sf::Time m_currentTime;
    float m_hue;
};

#endif // COLORGENERATOR_INCLUDE
