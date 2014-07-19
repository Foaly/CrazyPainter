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
    CCrazyPainter CrazyPainter;
    CrazyPainter.Init(window.getSize());

    // Create some veriables
    sf::Event Event;
    sf::Clock FrameClock;

    while (window.isOpen())
    {
        while (window.pollEvent(Event))
        {
            // Close Event
            if (Event.type == sf::Event::Closed)
                window.close();

            // Key Events
            if(Event.type == sf::Event::KeyPressed)
            {
                #ifdef DEBUG
                // Capture Screenshot
                if (Event.key.code == sf::Keyboard::F1)
                {
                    sf::Image Screen = window.capture();
                    Screen.saveToFile("screenshot.jpg");
                }
                #endif

                // Escape key pressed, close window
                if (Event.key.code == sf::Keyboard::Escape)
                    window.close();
            }

            CrazyPainter.HandleEvents(Event, window);
        }

        // move the painter
        CrazyPainter.Move(FrameClock.restart(), window);

//        std::cout << "Framerate: " << 1 / fFrameTime << std::endl;


        // Draw to the window
        window.clear();
        CrazyPainter.Render(window);
        window.display();
    }

    return 0;
}
