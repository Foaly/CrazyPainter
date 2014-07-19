
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "Utilities.h"


int main()
{
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Circle");

    window.setFramerateLimit(60);

    sf::RenderTexture m_RenderTexture;
    m_RenderTexture.create(window.getSize().x, window.getSize().y);
    sf::Sprite m_RenderSprite;
    sf::Vector2f HalfWindowSize(window.getSize() / 2u);

    sf::CircleShape circle;
    circle.setOutlineThickness(1);
    circle.setOutlineColor(sf::Color::Black);
    circle.setRadius(50);
    circle.setPosition(HalfWindowSize.x - circle.getRadius(), HalfWindowSize.y - circle.getRadius());

    sf::Vector2f RotationCenter(HalfWindowSize.x - circle.getRadius(), HalfWindowSize.y - circle.getRadius());
    srand(static_cast<unsigned int>(time(NULL)));
    float theta = 0.0;
    float OldRadius = 0.0;
    float Radius = OldRadius;
    float NewRadius = static_cast<float>(rand()) / RAND_MAX * (window.getSize().x / 2 - circle.getRadius());
    sf::Time m_TotalTime(sf::seconds(1.f));
    sf::Clock m_CurrentTime;



    sf::Clock FrameClock;
    float fTime;

    float hue = 0.0;
    sf::Color color;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close Event
            if (event.type == sf::Event::Closed)
                window.close();

            // Escape key pressed
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
                window.close();

        }

        fTime = FrameClock.restart().asSeconds();
//        std::cout << "Framerate: " << 1 / fTime << std::endl;

        // Calculate the color
        hue += 0.25 * fTime;
        if(hue > 1.0)
            hue = 0.0;

        float r = std::abs(3.0 - 6.0 * hue) - 1.0;
        float g = 2.0 - std::abs(2.0 - 6.0 * hue);
        float b = 2.0 - std::abs(4.0 - 6.0 * hue);

        color = sf::Color(Clamp(r, 0.f, 1.f) * 255.f, Clamp(g, 0.f, 1.f) * 255.f, Clamp(b, 0.f, 1.f) * 255.f);

        circle.setFillColor(color);


        theta += M_PI * fTime; // one complete circle in 2 seconds
        theta = fmod(theta, M_PI * 2);
//        if(theta > M_PI * 2)
//            theta -= M_PI * 2;
//        std::cout << "Theta: " << theta << std::endl;

        float amount = m_CurrentTime.getElapsedTime().asSeconds() / m_TotalTime.asSeconds();

        if(amount <= 1.f)
        {
            Radius = Linear_Interpolation(OldRadius, NewRadius, amount);

            sf::Vector2f PointToRotate(RotationCenter.x - Radius, RotationCenter.y);

            circle.setPosition(cos(theta) * (PointToRotate.x - RotationCenter.x) - sin(theta) * (PointToRotate.y - RotationCenter.y) + RotationCenter.x, sin(theta) * (PointToRotate.x - RotationCenter.x) + cos(theta) * (PointToRotate.y - RotationCenter.y) + RotationCenter.y);
        }
        else
        {
            OldRadius = NewRadius;
            NewRadius = static_cast<float>(rand()) / RAND_MAX * (window.getSize().x / 2 - circle.getRadius());
            m_CurrentTime.restart();
        }

        m_RenderTexture.draw(circle);
        m_RenderTexture.display();

        window.clear();
        m_RenderSprite.setTexture(m_RenderTexture.getTexture());
        window.draw(m_RenderSprite);
        window.display();
    }

    return 0;
}

