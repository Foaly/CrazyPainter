#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "RoundendedLine.h"
#include <iostream>
#include "Utilities.h"

#include "CrazyPainter.hpp"

int main()
{
    // Log some info
#ifdef DEBUG
    std::cout << "Maximale Texturgröße: " << sf::Texture::getMaximumSize() << std::endl;
    std::cout << "Grafikkartenhersteller: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Grafikkartenname: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "SFML Version: " << SFML_VERSION_MAJOR  << "." << SFML_VERSION_MINOR << std::endl;
    std::cout << "VideoMode:" << std::endl << "  Breite: " << sf::VideoMode::getFullscreenModes()[0].width  << std::endl <<  "  Höhe: " << sf::VideoMode::getFullscreenModes()[0].height  << std::endl <<  "  Bits per Pixel: " << sf::VideoMode::getFullscreenModes()[0].bitsPerPixel << std::endl;
#endif

    // Set up the window
#ifdef DEBUG
    sf::RenderWindow window(sf::VideoMode::getFullscreenModes()[0], "Crazy Painter");
#else
    sf::RenderWindow window(sf::VideoMode::getFullscreenModes()[0], "Crazy Painter", sf::Style::Fullscreen);
#endif
    window.setFramerateLimit(60);

    // Create the painter
    CrazyPainter crazyPainter;
    crazyPainter.Init(window.getSize());

    // Create some veriables
    sf::Event event;
    sf::Clock frameClock;

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            // Close Event
            if (event.type == sf::Event::Closed)
                window.close();

            // Key Events
            if(event.type == sf::Event::KeyPressed)
            {
                #ifdef DEBUG
                // Capture Screenshot
                if (event.key.code == sf::Keyboard::F1)
                {
                    sf::Image Screen = window.capture();
                    Screen.saveToFile("screenshot.jpg");
                }
                #endif

                // Escape key pressed, close window
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
            }

            crazyPainter.HandleEvents(event, window);
        }

        sf::Time frameTime = frameClock.restart();

        // move the painter
        crazyPainter.Move(frameTime, window);

//        std::cout << "Framerate: " << 1 / frameTime.asSeconds() << std::endl;


        // Draw to the window
        window.clear();
        crazyPainter.Render(window);
        window.display();
    }

    return 0;
}
