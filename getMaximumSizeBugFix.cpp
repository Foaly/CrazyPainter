#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
//#include <SFML/OpenGL.hpp>
#include <iostream>

// Shader Bug Test
int main()
{
    std::cout << "Maximale Texturgröße: " << sf::Texture::getMaximumSize() << std::endl;
//    std::cout << "Grafikkartenhersteller: " << glGetString(GL_VENDOR) << std::endl;
//    std::cout << "Grafikkartenname: " << glGetString(GL_RENDERER) << std::endl;
//    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
//    std::cout << "SFML Version: " << SFML_VERSION_MAJOR  << "." << SFML_VERSION_MINOR << std::endl;

     // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
    window.setFramerateLimit(60);

    if(!sf::Shader::isAvailable()) {
        std::cout << "Shader not available!" << std::endl;
    }

    sf::Shader m_FadeShader;
    m_FadeShader.loadFromFile("FadeShader.frag", sf::Shader::Fragment);
    m_FadeShader.setParameter("fRate", 0.5f);

    sf::RectangleShape rect(sf::Vector2f(200, 200));
    rect.setPosition(300, 200);

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
        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...
        window.draw(rect, &m_FadeShader);

        // end the current frame
        window.display();
    }

    return 0;
}

