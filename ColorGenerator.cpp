#include "ColorGenerator.hpp"
#include "Random.hpp"
#include <iostream>


ColorGenerator::ColorGenerator() : m_startColor(getRandomColor()),
                                     m_endColor(getRandomColor()),
                                     m_duration(sf::seconds(5.f)),
                                     m_currentTime(sf::Time::Zero),
                                     m_hue(random(0.f, 1.f))
{

}

// interpolates between randomly generated colors
sf::Color ColorGenerator::generateInterpolatedColor(sf::Time deltaTime)
{
    m_currentTime += deltaTime;
    if(m_currentTime > m_duration)
    {
        m_currentTime = sf::microseconds(m_currentTime.asMicroseconds() % m_duration.asMicroseconds());
        m_startColor = m_endColor;
        m_endColor = getRandomColorLessGray();
    }

    float amount = m_currentTime.asSeconds() / m_duration.asSeconds();
    sf::Uint8 r = Linear_Interpolation(m_startColor.r, m_endColor.r, amount);
    sf::Uint8 g = Linear_Interpolation(m_startColor.g, m_endColor.g, amount);
    sf::Uint8 b = Linear_Interpolation(m_startColor.b, m_endColor.b, amount);

    return sf::Color(r, g, b);
}

sf::Color ColorGenerator::generateHueShiftedColor(float hueStep)
{
    // compute a shifting hue
    m_hue += hueStep;
    if(m_hue > 1.f)
        m_hue = std::fmod(m_hue, 1.f);

    return hueToRGB(m_hue);
}

sf::Color ColorGenerator::getRandomColor()
{
    return sf::Color(random(0, 255), random(0, 255), random(0, 255));
}

sf::Color ColorGenerator::getRandomHue()
{
    return hueToRGB(random(0.f, 1.f));
}

// random color with less gray
sf::Color ColorGenerator::getRandomColorLessGray()
{
    unsigned char color = random(0, 255);
    switch(random(0, 5))
    {
        case 0:
            return sf::Color(255, color, 0);
        case 1:
            return sf::Color(255, 0, color);
        case 2:
            return sf::Color(0, 255, color);
        case 3:
            return sf::Color(color, 255, 0);
        case 4:
            return sf::Color(color, 0, 255);
        case 5:
            return sf::Color(0, color, 255);
        default:
            return sf::Color(); // this will never occur, but it keeps the compiler happy
    }
}

// hue has to be in range [0.f;1.f]
sf::Color ColorGenerator::hueToRGB(float hue)
{
    // Convert hue to RGB
    float r = std::abs(3.f - 6.f * hue) - 1.f;
    float g = 2.f - std::abs(2.f - 6.f * hue);
    float b = 2.f - std::abs(4.f - 6.f * hue);

    return sf::Color(static_cast<sf::Uint8>(clamp(r, 0.f, 1.f) * 255), static_cast<sf::Uint8>(clamp(g, 0.f, 1.f) * 255), static_cast<sf::Uint8>(clamp(b, 0.f, 1.f) * 255));
}
