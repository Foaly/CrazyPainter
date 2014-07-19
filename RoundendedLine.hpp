#ifndef ROUNDENDEDLINE_H
#define ROUNDENDEDLINE_H

#include <SFML/Graphics/Shape.hpp>

#ifndef PI
    #define PI 3.1415926535897932384626433832795f
#endif


class RoundendedLine : public sf::Shape
{
public:

    RoundendedLine(const sf::Vector2f& endPoint = sf::Vector2f(0, 0), const float width = 1.0);

    void setStartPoint(float x, float y);
    void setStartPoint(const sf::Vector2f& position);
    const sf::Vector2f& getStartPoint() const;

    void setEndPoint(float x, float y);
    void setEndPoint(const sf::Vector2f& endPoint);
    const sf::Vector2f& getEndPoint() const;

    void setWidth(const float width);

    virtual unsigned int getPointCount() const;
    virtual sf::Vector2f getPoint(unsigned int index) const;


private :

    // make these methods private to hide them and force the user to use
    // the more expressive methods setStartPoint() and getStartPoint()
    void setPosition(const sf::Vector2f& position);
    void setPosition(float x, float y);
    const sf::Vector2f& getPosition() const;

    sf::Vector2f m_endPoint;
    float m_width;
};

#endif //ROUNDENDEDLINE_H
