#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
//#include <SFML/OpenGL.hpp>
#include <iostream>

// subtractive blend mode Test
int main()
{
     // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
    window.setFramerateLimit(60);

    sf::CircleShape circle(100);
    circle.setPosition(300, 200);
    circle.setFillColor(sf::Color::Red);

    sf::RectangleShape rect(sf::Vector2f(800, 600));
    rect.setFillColor(sf::Color(0, 0, 0, 1));

    const sf::BlendMode blendMode(sf::BlendMode::SrcAlpha, sf::BlendMode::OneMinusSrcAlpha, sf::BlendMode::Subtract,
                                  sf::BlendMode::One, sf::BlendMode::OneMinusSrcAlpha, sf::BlendMode::Subtract);

    sf::RenderTexture renderTexture;
    renderTexture.create(800, 600);
    renderTexture.clear();
    renderTexture.draw(circle);
    renderTexture.display();

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();


            // Escape key pressed, close window
            if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                window.close();
        }

        renderTexture.draw(rect, blendMode);
        renderTexture.display();



        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...
        window.draw(sf::Sprite(renderTexture.getTexture()));

        // end the current frame
        window.display();
    }

    return 0;
}
