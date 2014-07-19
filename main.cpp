//#include <SFML/System.hpp>
//#include <SFML/Window.hpp>
//#include <SFML/Graphics.hpp>
//#include <SFML/OpenGL.hpp>
//#include "RoundendedLine.h"
//#include <iostream>
//#include "Utilities.h"
//
//namespace Interpolation
//{
//    enum InterplationType
//    {
//        Smoothstep,
//        Hermite,
//        Jitter
//    };
//}
//
//int main()
//{
//    // Log some info
//#ifdef DEBUG
//    std::wcout << "Maximale Texturgröße: " << sf::Texture::getMaximumSize() << std::endl;
//    std::cout << "Grafikkartenhersteller: " << glGetString(GL_VENDOR) << std::endl;
//    std::cout << "Grafikkartenname: " << glGetString(GL_RENDERER) << std::endl;
//    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
//    std::cout << "SFML Version: " << SFML_VERSION_MAJOR  << "." << SFML_VERSION_MINOR << std::endl;
//#endif
//
//    // Set up the window
//#ifdef DEBUG
//    sf::RenderWindow window(sf::VideoMode::getFullscreenModes()[0], "Crazy Painter");
//#else
//    sf::RenderWindow window(sf::VideoMode::getFullscreenModes()[0], "Crazy Painter", sf::Style::Fullscreen);
//#endif
//    window.setFramerateLimit(60);
//
//    // Create Rendertextures and fade mechanism
//    sf::RenderTexture targets[2];
//    targets[0].create(window.getSize().x, window.getSize().y);
//    targets[1].create(window.getSize().x, window.getSize().y);
//    sf::RenderTexture* front = targets;
//    sf::RenderTexture* back = &targets[1];
//
//    sf::Sprite m_RenderSprite;
//    sf::Shader FadeShader;
//    FadeShader.loadFromFile("FadeShader.frag", sf::Shader::Fragment);
//    FadeShader.setParameter("texture", sf::Shader::CurrentTexture);
//    sf::RectangleShape FadeRect(sf::Vector2f(window.getSize()));
//    FadeRect.setFillColor(sf::Color(0, 0, 0, 10));
//
//    // Create a little watermark
//    sf::Font Font;
//    Font.loadFromFile("LiberationSans-Regular.ttf");
//
//    sf::Text AuthorName;
//    AuthorName.setString("Maximilian Wagenbach");
//    AuthorName.setFont(Font);
//    AuthorName.setCharacterSize(40);
//    AuthorName.setColor(sf::Color(54, 54, 54));
//    AuthorName.setPosition(window.getSize().x - AuthorName.getLocalBounds().width - 10, window.getSize().y - AuthorName.getLocalBounds().height - 10);
//
//    // Set up some controls
//    bool bfade = false;
//    bool bAuto = false;
//    bool bWatermark = false;
//    Interpolation::InterplationType InterpolationType = Interpolation::Hermite;
//    sf::Vector2f HalfWindowSize(window.getSize() / 2u);
//
//    // Lookup Tables
//    float Sin_Lookup [8];
//    float Cos_Lookup [8];
//
//    for(int i = 0; i < 8; i++)
//    {
//        Sin_Lookup[i] = sin(i * M_PI / 4);
//        Cos_Lookup[i] = cos(i * M_PI / 4);
//    }
//
//    // Initialize the lines
//    CRoundendedLine Lines [16];
//
//    for(int i = 0; i < 16; i++)
//    {
//        Lines[i].setPosition(HalfWindowSize);
//        Lines[i].setEndPoint(HalfWindowSize);
//        Lines[i].setWidth(5);
//        Lines[i].setFillColor(sf::Color::Red);
//    }
//
//    // Setup variables for the interpolation
//    sf::Clock FrameClock;
//    float fFrameTime;
//    sf::Vector2f OldPosition = HalfWindowSize;
//    srand(static_cast<unsigned int>(time(NULL)));
//    sf::Vector2f m_StartPosition(HalfWindowSize);
//    sf::Vector2f m_EndPosition((static_cast<float>(rand()) / RAND_MAX * window.getSize().x), (static_cast<float>(rand()) / RAND_MAX * window.getSize().y));
//    sf::Vector2f m_NextPosition((static_cast<float>(rand()) / RAND_MAX * window.getSize().x), (static_cast<float>(rand()) / RAND_MAX * window.getSize().y));
//    sf::Vector2f m_StartTangent(0.f, 0.f);
//    sf::Vector2f m_EndTangent((m_NextPosition - m_StartPosition)); // Catmull-Rom Spline (Faktor = 1)
//    sf::Time m_TotalTime(sf::seconds(1.f));
//    sf::Clock m_CurrentTime;
//
//    // Intialize the color
//    float hue = 0.0;
//    sf::Color color;
//
//    while (window.isOpen())
//    {
//        sf::Event event;
//        while (window.pollEvent(event))
//        {
//            // Close Event
//            if (event.type == sf::Event::Closed)
//                window.close();
//
//            // Key Events
//            if(event.type == sf::Event::KeyPressed)
//            {
//                #ifdef DEBUG
//                // Capture Screenshot
//                if (event.key.code == sf::Keyboard::F1)
//                {
//                    sf::Image Screen = window.capture();
//                    Screen.saveToFile("screenshot.jpg");
//                }
//                #endif
//
//                // Escape key pressed, close window
//                if (event.key.code == sf::Keyboard::Escape)
//                    window.close();
//
//                // Clear RenderTexture
//                else if (event.key.code == sf::Keyboard::C)
//                    back->clear();
//
//                // Toggle RenderTexture fading
//                else if (event.key.code == sf::Keyboard::F)
//                    bfade = !bfade;
//
//                // Toggle Watermark
//                else if (event.key.code == sf::Keyboard::W)
//                    bWatermark = !bWatermark;
//
//                // Toggle Automatic Mode
//                else if (event.key.code == sf::Keyboard::A)
//                {
//                    bAuto = !bAuto;
//                    window.setMouseCursorVisible(!bAuto);
//                    m_StartPosition = OldPosition;
//                    m_CurrentTime.restart();
//                }
//
//                // Switch between interpolation modes
//                else if (event.key.code == sf::Keyboard::Num1)
//                    InterpolationType = Interpolation::Hermite;
//
//                else if (event.key.code == sf::Keyboard::Num2)
//                    InterpolationType = Interpolation::Smoothstep;
//
//                else if (event.key.code == sf::Keyboard::Num3)
//                {
//                    InterpolationType = Interpolation::Jitter;
//
//                    for(int i = 0; i < 16; i++)
//                    {
//                        Lines[i].setPosition(HalfWindowSize);
//                        Lines[i].setEndPoint(HalfWindowSize);
//                    }
//
//                    OldPosition = sf::Vector2f(HalfWindowSize);
//                }
//
//            }
//
//            // Reset on MouseRelease
//            if ((event.type == sf::Event::MouseButtonReleased) && (event.mouseButton.button == sf::Mouse::Left) && !bAuto)
//            {
//                for(int i = 0; i < 16; i++)
//                {
//                    Lines[i].setPosition(HalfWindowSize);
//                    Lines[i].setEndPoint(HalfWindowSize);
//                }
//
//                OldPosition = sf::Vector2f(HalfWindowSize);
//            }
//        }
//
//        fFrameTime = FrameClock.restart().asSeconds();
////        std::cout << "Framerate: " << 1 / fFrameTime << std::endl;
//
//        // Calculate the color
//        hue += 0.25 * fFrameTime;
//        if(hue > 1.0)
//            hue = 0.0;
//
//        float r = std::abs(3.0 - 6.0 * hue) - 1.0;
//        float g = 2.0 - std::abs(2.0 - 6.0 * hue);
//        float b = 2.0 - std::abs(4.0 - 6.0 * hue);
//
//        color = sf::Color(Clamp(r, 0.f, 1.f) * 255.f, Clamp(g, 0.f, 1.f) * 255.f, Clamp(b, 0.f, 1.f) * 255.f);
//
//        // Automatic Mode
//        if(bAuto)
//        {
//            float amount = m_CurrentTime.getElapsedTime().asSeconds() / m_TotalTime.asSeconds();
//
//            if(amount < 1.f)
//            {
//                sf::Vector2f interpolation;
//
//                switch (InterpolationType)
//                {
//                    case Interpolation::Hermite:
//                        interpolation = Hermite_Interpolation(m_StartPosition, m_StartTangent, m_EndPosition, m_EndTangent, amount);
//                        break;
//
//                    case Interpolation::Smoothstep:
//                        interpolation = Smoothstep_x2_Interpolation(m_StartPosition, m_EndPosition, amount);
//                        break;
//
//                    case Interpolation::Jitter:
//                        interpolation = sf::Vector2f(OldPosition.x + static_cast<float>(rand()) / RAND_MAX * 20 - 10, OldPosition.y + static_cast<float>(rand()) / RAND_MAX * 20 -10);
//                        break;
//                }
//
//                for(int i = 0; i < 8; i++)
//                {
//                    Lines[i].setPosition(Lines[i].getEndPoint());
//                    Lines[i].setEndPoint(sf::Vector2f(Cos_Lookup[i] * (interpolation.x - HalfWindowSize.x) - Sin_Lookup[i] * (interpolation.y - HalfWindowSize.y) + HalfWindowSize.x, Sin_Lookup[i] * (interpolation.x - HalfWindowSize.x) + Cos_Lookup[i] * (interpolation.y - HalfWindowSize.y) + HalfWindowSize.y));
//                    Lines[i].setFillColor(color);
//
//                    Lines[i + 8].setPosition(Lines[i + 8].getEndPoint());
//                    Lines[i + 8].setEndPoint(sf::Vector2f(Cos_Lookup[i] * (interpolation.x - HalfWindowSize.x) * - 1 - Sin_Lookup[i] * (interpolation.y - HalfWindowSize.y) + HalfWindowSize.x, Sin_Lookup[i] * (interpolation.x - HalfWindowSize.x) * - 1 + Cos_Lookup[i] * (interpolation.y - HalfWindowSize.y) + HalfWindowSize.y));
//                    Lines[i + 8].setFillColor(color);
//                }
//
//
//                OldPosition = interpolation;
//            }
//            else
//            {
//                // interpolation is finished, reset variables
//                m_StartPosition = m_EndPosition;
//
//                switch (InterpolationType)
//                {
//                    case Interpolation::Hermite:
//                        m_StartTangent = m_EndTangent;
//                        m_EndPosition = m_NextPosition;
//                        m_EndTangent = sf::Vector2f((m_NextPosition - m_StartPosition));
//                        m_EndPosition = sf::Vector2f((static_cast<float>(rand()) / RAND_MAX * window.getSize().x), (static_cast<float>(rand()) / RAND_MAX * window.getSize().y));
//                        break;
//
//                    case Interpolation::Smoothstep:
//                        m_EndPosition = sf::Vector2f((static_cast<float>(rand()) / RAND_MAX * window.getSize().x), (static_cast<float>(rand()) / RAND_MAX * window.getSize().y));
//                        break;
//
//                    default:
//                        break;
//                }
//
//                m_CurrentTime.restart();
//            }
//        }
//        // Manuel Mode
//        else if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
//        {
//            sf::Vector2f MousePosition = sf::Vector2f(sf::Mouse::getPosition(window));
//
//            for(int i = 0; i < 8; i++)
//            {
//                Lines[i].setPosition(Lines[i].getEndPoint());
//                Lines[i].setEndPoint(sf::Vector2f(Cos_Lookup[i] * (MousePosition.x - HalfWindowSize.x) - Sin_Lookup[i] * (MousePosition.y - HalfWindowSize.y) + HalfWindowSize.x, Sin_Lookup[i] * (MousePosition.x - HalfWindowSize.x) + Cos_Lookup[i] * (MousePosition.y - HalfWindowSize.y) + HalfWindowSize.y));
//                Lines[i].setFillColor(color);
//
//                Lines[i + 8].setPosition(Lines[i + 8].getEndPoint());
//                Lines[i + 8].setEndPoint(sf::Vector2f(Cos_Lookup[i] * (MousePosition.x - HalfWindowSize.x) * - 1 - Sin_Lookup[i] * (MousePosition.y - HalfWindowSize.y) + HalfWindowSize.x, Sin_Lookup[i] * (MousePosition.x - HalfWindowSize.x) * - 1 + Cos_Lookup[i] * (MousePosition.y - HalfWindowSize.y) + HalfWindowSize.y));
//                Lines[i + 8].setFillColor(color);
//            }
//
//
//            OldPosition = MousePosition;
//        }
//
//        // Draw everything to the RenderTexture
//        front->clear(sf::Color::Transparent);
//        if(bfade)
//        {
//            FadeShader.setParameter("fRate", fFrameTime  * 0.12); // or * 0.112 for slow fade
//            front->draw(sf::Sprite(back->getTexture()), &FadeShader);
//        }
//        else
//            front->draw(sf::Sprite(back->getTexture()));
//
//        for(int i = 0; i < 16; i++)
//        {
//            front->draw(Lines[i]);
//        }
//        front->display();
//        swap(front, back);
//
//
//
//        // Draw to the window
//        window.clear();
//        m_RenderSprite.setTexture(back->getTexture());
//        window.draw(m_RenderSprite);
//        if(bWatermark)
//            window.draw(AuthorName);
//        window.display();
//    }
//
//    return 0;
//}
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
