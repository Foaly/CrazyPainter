#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Err.hpp>
#include <SFML/Window/Context.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <fstream>
#include <chrono>


#include "CrazyPainter.hpp"
#include "PathResolver.hpp"

int main()
{
    // measure the time since the start of the program
	std::chrono::system_clock::time_point startOfProgramTimePoint = std::chrono::system_clock::now();
    std::time_t startTime_t = std::chrono::system_clock::to_time_t(startOfProgramTimePoint);

#ifndef DEBUG
    // create a file name containing the current date and time so logs dont get overwritten
    char timeDate [21];
    std::strftime(timeDate, sizeof(timeDate), "%d %b %Y %H.%M.%S", std::localtime(&startTime_t));

    std::string logFileName = resolvePath("logs/" + std::string(timeDate) + ".log");

    // in release build redirect the errors to a log, so we can view them later
    std::ofstream log (logFileName);
    //save old buffer
    auto cinbuf = std::cin.rdbuf();
    auto coutbuf = std::cout.rdbuf();
    if (log.is_open())
    {
        std::cout << "Log was opened successfully. All further info is logged in: " << logFileName << std::endl;

        sf::err().rdbuf(log.rdbuf()); // redirect sf::err to the log
        std::cerr.rdbuf(log.rdbuf()); // redirect std::cerr to the log
        std::cout.rdbuf(log.rdbuf()); // redirect std::cout to the log
    }
#endif

	// log the start time
    std::cout << "Programm started at: " << std::ctime(&startTime_t) << std::endl;

    // Log some info
    {
        sf::Context context;
        std::cout << "Graphicscard vendor: " << glGetString(GL_VENDOR) << std::endl;
        std::cout << "Graphicscard name: " << glGetString(GL_RENDERER) << std::endl;
        std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    }
    std::cout << "Compiled with SFML Version: " << SFML_VERSION_MAJOR  << "." << SFML_VERSION_MINOR << std::endl;
    std::cout << "VideoMode:" << std::endl << "  Width: " << sf::VideoMode::getFullscreenModes()[0].width  << std::endl <<  "  Height: " << sf::VideoMode::getFullscreenModes()[0].height  << std::endl <<  "  Bits per Pixel: " << sf::VideoMode::getFullscreenModes()[0].bitsPerPixel << std::endl;
    std::cout << "Maximum Textursize: " << sf::Texture::getMaximumSize() << std::endl;

    // Set up the window
    sf::ContextSettings contextSettings;
    contextSettings.antialiasingLevel = 16;
#ifdef DEBUG
    sf::RenderWindow window(sf::VideoMode::getFullscreenModes()[0], "Crazy Painter", sf::Style::Default, contextSettings);
#else
    sf::RenderWindow window(sf::VideoMode::getFullscreenModes()[0], "Crazy Painter", sf::Style::Fullscreen, contextSettings);
#endif
    window.setFramerateLimit(60);

    // Create the painter
    CrazyPainter crazyPainter(window);

    // Create some variables
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
            if(event.type == sf::Event::KeyReleased)
            {
                #ifdef DEBUG
                // Capture Screenshot
                if (event.key.code == sf::Keyboard::F1)
                {
                    sf::Vector2u windowSize = window.getSize();
                    sf::Texture texture;
                    texture.create(windowSize.x, windowSize.y);
                    texture.update(window);
                    sf::Image screenshot = texture.copyToImage();
                    screenshot.saveToFile("screenshot.jpg");
                }
                #endif

                // Escape key pressed, close window
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
            }

            crazyPainter.handleEvents(event, window);
        }

        sf::Time frameTime = frameClock.restart();

        // move the painter
        crazyPainter.update(frameTime);

//        std::cout << "Framerate: " << 1 / frameTime.asSeconds() << std::endl;


        // Draw to the window
        window.clear();
        crazyPainter.render(window);
        window.display();
    }

    // Print out how long the program has been running
    std::chrono::system_clock::time_point endOfProgramTimePoint = std::chrono::system_clock::now();

    std::chrono::duration<double> runTimeInSeconds = std::chrono::duration_cast<std::chrono::duration<double>>(endOfProgramTimePoint - startOfProgramTimePoint);
    std::cout << std::endl << "The program has been running for: " << static_cast<long>(std::floor(runTimeInSeconds.count())) / 60 << " minutes and " << std::fmod(runTimeInSeconds.count(), 60) << " seconds." << std::endl;


#ifndef DEBUG
    // reset to standard input/output again
    std::cin.rdbuf(cinbuf);
    std::cout.rdbuf(coutbuf);
#endif

    return 0;
}
