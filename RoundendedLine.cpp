#include "RoundendedLine.hpp"

RoundendedLine::RoundendedLine(const sf::Vector2f& endPoint, const float width) :
    m_endPoint (endPoint),
    m_width (width)

{
    update();
}

void RoundendedLine::setStartPoint(float x, float y)
{
    Transformable::setPosition(x, y);
}

void RoundendedLine::setStartPoint(const sf::Vector2f& position)
{
    setStartPoint(position.x, position.y);
}

const sf::Vector2f& RoundendedLine::getStartPoint() const
{
    return Transformable::getPosition();
}

void RoundendedLine::setEndPoint(float x, float y)
{
    m_endPoint = sf::Vector2f(x, y);
    update();
}

void RoundendedLine::setEndPoint(const sf::Vector2f& endPoint)
{
    m_endPoint = endPoint;
    update();
}

const sf::Vector2f& RoundendedLine::getEndPoint() const
{
    return m_endPoint;
}

void RoundendedLine::setWidth(const float width)
{
    m_width = width;
    update();
}

unsigned int RoundendedLine::getPointCount() const
{
    return 30;
}

sf::Vector2f RoundendedLine::getPoint(unsigned int index) const
{
//    std::cout << "beeing called!";
    sf::Vector2f P1(1.0, 0.0);
    sf::Vector2f P2(m_endPoint + sf::Vector2f(1.0, 0.0) - Transformable::getPosition());

    sf::Vector2f offset;
    int iFlipDirection;

    if(index < 15)
    {
        offset = P2;
        iFlipDirection = 1;
    }
    else
    {
        offset = P1;
        iFlipDirection = -1;
        index -= 15;
    }

    float start = -atan2(P1.y - P2.y, P2.x - P1.x);

    float angle = index * PI / 14 - PI / 2 + start;
    float x = std::cos(angle) * m_width / 2;
    float y = std::sin(angle) * m_width / 2;

    return sf::Vector2f(offset.x + x * iFlipDirection, offset.y + y * iFlipDirection);
}
