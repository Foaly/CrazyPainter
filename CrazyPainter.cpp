#include "CrazyPainter.hpp"
#include "Utilities.hpp"
#include "Random.hpp"

CrazyPainter::CrazyPainter(sf::RenderWindow& window)
{
    // save a reference to the window, to get mouse input
    m_window = &window;

    // Get the Targets size
    m_TargetSize = sf::Vector2f(m_window->getSize());

    // Create Rendertextures
    m_Targets[0].create(m_TargetSize.x, m_TargetSize.y);
    m_Targets[1].create(m_TargetSize.x, m_TargetSize.y);
    m_FrontTarget = m_Targets;
    m_BackTarget = &m_Targets[1];

    // Set up the fade shader
    m_FadeShader.loadFromFile("FadeShader.frag", sf::Shader::Fragment);
    m_FadeShader.setParameter("texture", sf::Shader::CurrentTexture);

    // Load a font and set a string as a "watermark"
    m_Font.loadFromFile("LiberationSans-Regular.ttf");

    m_AuthorName.setString("Maximilian Wagenbach");
    m_AuthorName.setFont(m_Font);
    m_AuthorName.setCharacterSize(40);
    m_AuthorName.setColor(sf::Color(54, 54, 54));
    m_AuthorName.setPosition(m_TargetSize.x - m_AuthorName.getLocalBounds().width - 10, m_TargetSize.y - m_AuthorName.getLocalBounds().height - 10);

    // Set up some controls
    m_bFade = false;
    m_bAutoDrawing = false;
    m_bAutoSwitching = false;
    m_bWatermark = false;
    m_fFadeSpeed = 0.1112;
    m_InterpolationType = Interpolation::Hermite;
    m_HalfTargetSize = m_TargetSize / 2.f;
    m_AutoSwitchingDuration = sf::seconds(60);

    // Initialize the sin/cos lookup tables
    for(int i = 0; i < 8; i++)
    {
        Sin_Lookup[i] = sin(i * PI / 4);
        Cos_Lookup[i] = cos(i * PI / 4);
    }

    // Initialize the lines
    resetLines();
    for(int i = 0; i < 16; i++)
    {
        m_Lines[i].setWidth(5);
        m_Lines[i].setFillColor(sf::Color::Red);
    }

    // Setup variables needed for the interpolation
    m_TotalTime = sf::seconds(1.f);
    m_StartPosition = m_HalfTargetSize;
    m_EndPosition = sf::Vector2f(random(0.f, m_TargetSize.x), random(0.f, m_TargetSize.y));


    // Setup variables needed for the hermite interpolation
    m_NextPosition = sf::Vector2f(random(0.f, m_TargetSize.x), random(0.f, m_TargetSize.y));
    m_StartTangent = sf::Vector2f(0.f, 0.f);
    m_EndTangent = m_NextPosition - m_StartPosition; // Catmull-Rom Spline (Faktor = 1)
}

void CrazyPainter::update(sf::Time FrameTime)
{
    m_FrameTime = FrameTime;

    if(m_bAutoSwitching)
        if(m_AutoSwitchingClock.getElapsedTime() >= m_AutoSwitchingDuration)
            changeInterpolationMode(1);

    // get the new hue shifted color
    m_Color = m_colorGenerator.generateHueShiftedColor(0.25 * m_FrameTime.asSeconds());

    // automatic drawing mode
    if(m_bAutoDrawing)
    {
        float amount = m_CurrentTime.getElapsedTime().asSeconds() / m_TotalTime.asSeconds();

        if(amount < 1.f)
        {
            sf::Vector2f interpolation;

            switch (m_InterpolationType)
            {
            case Interpolation::Hermite:
                interpolation = Hermite_Interpolation(m_StartPosition, m_StartTangent, m_EndPosition, m_EndTangent, amount);
                break;

            case Interpolation::Smoothstep:
                interpolation = Smoothstep_x2_Interpolation(m_StartPosition, m_EndPosition, amount);
                break;

            case Interpolation::Jitter:
                interpolation = sf::Vector2f(m_OldPosition.x + random(-10.f, 10.f), m_OldPosition.y + random(-10.f, 10.f));
                break;

            default:
                break;
            }

            calculateLines(interpolation);
        }
        else
        {
            // interpolation is finished, reset variables
            m_StartPosition = m_EndPosition;

            switch (m_InterpolationType)
            {
            case Interpolation::Hermite:
                m_StartTangent = m_EndTangent;
                m_EndPosition = m_NextPosition;
                m_NextPosition = sf::Vector2f(random(0.f, m_TargetSize.x), random(0.f, m_TargetSize.y));
                m_EndTangent = sf::Vector2f(m_NextPosition - m_StartPosition);
                break;

            case Interpolation::Smoothstep:
                m_EndPosition = sf::Vector2f(random(0.f, m_TargetSize.x), random(0.f, m_TargetSize.y));
                break;

            case Interpolation::Jitter:
            {
                sf::Vector2f m_currentPosition = m_Lines[0].getEndPoint();
                // if the points are outside the target, reset them to the center
                if(m_currentPosition.x < 0 || m_currentPosition.x > m_TargetSize.x || m_currentPosition.y < 0 || m_currentPosition.y > m_TargetSize.y)
                    resetLines();
            }

            default:
                break;
            }

            m_CurrentTime.restart();
        }
    }
    // Manual Mode
    else if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        // get the mouse position (pixel are mapped to world coordinates, to take resizing the window into account)
        sf::Vector2f mousePosition = m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window));

        calculateLines(mousePosition);
    }

}

void CrazyPainter::render(sf::RenderTarget& Target)
{
    // Draw everything to the RenderTexture
    m_FrontTarget->clear(sf::Color::Transparent);
    if(m_bFade)
    {
        m_FadeShader.setParameter("fRate", m_FrameTime.asSeconds()  * m_fFadeSpeed);
        m_FrontTarget->draw(sf::Sprite(m_BackTarget->getTexture()), &m_FadeShader);
    }
    else
        m_FrontTarget->draw(sf::Sprite(m_BackTarget->getTexture()));

    for(int i = 0; i < 16; i++)
    {
        m_FrontTarget->draw(m_Lines[i]);
    }
    m_FrontTarget->display();
    std::swap(m_FrontTarget, m_BackTarget);



    // Draw to the window
    m_RenderSprite.setTexture(m_BackTarget->getTexture());
    Target.draw(m_RenderSprite);
    if(m_bWatermark)
        Target.draw(m_AuthorName);
}

void CrazyPainter::handleEvents(sf::Event& event, sf::Window& window)
{
    // Key Events
    if(event.type == sf::Event::KeyPressed)
    {
        switch(event.key.code)
        {
        case sf::Keyboard::C:
            // Clear RenderTexture
            m_BackTarget->clear();
            break;
        case sf::Keyboard::F:
            // Toggle RenderTexture fading
            m_bFade = !m_bFade;
            break;
        case sf::Keyboard::W:
            // Toggle Watermark
            m_bWatermark = !m_bWatermark;
            break;
        case sf::Keyboard::A:
            // Toggle Automatic drawing Mode
            m_bAutoDrawing = !m_bAutoDrawing;
            window.setMouseCursorVisible(!m_bAutoDrawing);
            m_StartPosition = m_OldPosition;
            m_CurrentTime.restart();
            resetLines();
            break;
        case sf::Keyboard::S:
            // toggle automatic switching between interpolation modes
            m_AutoSwitchingClock.restart();
            m_bAutoSwitching = !m_bAutoSwitching;
            break;
        case sf::Keyboard::Right:
            // advance to the next interpolation mode
            changeInterpolationMode(1);
            break;
        case sf::Keyboard::Left:
            // go to the previous interpolation mode
            changeInterpolationMode(-1);
            break;
        // Switch between interpolation modes
        case sf::Keyboard::Num1:
            m_InterpolationType = Interpolation::Hermite;
            break;
        case sf::Keyboard::Num2:
            m_InterpolationType = Interpolation::Smoothstep;
            break;
        case sf::Keyboard::Num3:
            m_InterpolationType = Interpolation::Jitter;
            resetLines();
            break;
        default:
            break;
        }
    }
    // Reset on MouseRelease
    else if ((event.type == sf::Event::MouseButtonReleased) && (event.mouseButton.button == sf::Mouse::Left) && !m_bAutoDrawing)
        resetLines();
}

void CrazyPainter::changeInterpolationMode(int step)
{
    m_AutoSwitchingClock.restart();

    if(step == 1)
        ++m_InterpolationType;
    else if(step == -1)
        --m_InterpolationType;

    if(m_bAutoSwitching)
    {
        if(m_InterpolationType == Interpolation::Jitter)
        {
            m_bFade = false;
            resetLines();
            m_BackTarget->clear();
        }
        else
        {
            m_bFade = true;

//            std::mt19937 generator(44687321);
//            std::bernoulli_distribution equalBoolDistribution(0.5);
//
//            bool randomBool = equalBoolDistribution(generator);
//
//            if(randomBool == true)
                m_fFadeSpeed = 0.1112; // slow fading
//            else
//                m_fFadeSpeed = 0.2; // fast Fading
        }
    }
}

void CrazyPainter::resetLines()
{
    for(int i = 0; i < 16; i++)
    {
        m_Lines[i].setStartPoint(m_HalfTargetSize);
        m_Lines[i].setEndPoint(m_HalfTargetSize);
    }

    m_OldPosition = m_HalfTargetSize;
}

void CrazyPainter::calculateLines(const sf::Vector2f nextPoint)
{
    for(int i = 0; i < 8; i++)
    {
        m_Lines[i].setStartPoint(m_Lines[i].getEndPoint());
        m_Lines[i].setEndPoint(sf::Vector2f(Cos_Lookup[i] * (nextPoint.x - m_HalfTargetSize.x) - Sin_Lookup[i] * (nextPoint.y - m_HalfTargetSize.y) + m_HalfTargetSize.x, Sin_Lookup[i] * (nextPoint.x - m_HalfTargetSize.x) + Cos_Lookup[i] * (nextPoint.y - m_HalfTargetSize.y) + m_HalfTargetSize.y));
        m_Lines[i].setFillColor(m_Color);

        m_Lines[i + 8].setStartPoint(m_Lines[i + 8].getEndPoint());
        m_Lines[i + 8].setEndPoint(sf::Vector2f(Cos_Lookup[i] * (nextPoint.x - m_HalfTargetSize.x) * - 1 - Sin_Lookup[i] * (nextPoint.y - m_HalfTargetSize.y) + m_HalfTargetSize.x, Sin_Lookup[i] * (nextPoint.x - m_HalfTargetSize.x) * - 1 + Cos_Lookup[i] * (nextPoint.y - m_HalfTargetSize.y) + m_HalfTargetSize.y));
        m_Lines[i + 8].setFillColor(m_Color);
    }

    m_OldPosition = nextPoint;
}
