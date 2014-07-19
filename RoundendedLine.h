#ifndef ROUNDENDEDLINE_H
#define ROUNDENDEDLINE_H

#include <SFML/Graphics/Shape.hpp>
#include <cmath>

#ifndef PI
    #define PI 3.1415926535897932384626433832795f
#endif


class CRoundendedLine : public sf::Shape
{
public:

    CRoundendedLine(const sf::Vector2f& endPoint = sf::Vector2f(0, 0), const float width = 1.0);

    void setEndPoint(const sf::Vector2f& endPoint);

    const sf::Vector2f& getEndPoint() const;

    void setWidth(const float width);

    virtual unsigned int getPointCount() const;

    virtual sf::Vector2f getPoint(unsigned int index) const;

private :

    sf::Vector2f m_endPoint;
    float m_Width;
};

#endif //ROUNDENDEDLINE_H
