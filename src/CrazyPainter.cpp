#include "CrazyPainter.hpp"
#include "Utilities.hpp"
#include "Random.hpp"
#include "PathResolver.hpp"

#include "HermiteInterpolation.hpp"
#include "SmoothstepInterpolation.hpp"
#include "JitterMode.hpp"
#include "CircleInterpolation.hpp"


CrazyPainter::CrazyPainter(sf::RenderWindow& window)
{
    // save a reference to the window, to get mouse input
    m_window = &window;

    // Get the Targets size
    m_targetSize = sf::Vector2f(m_window->getSize());

    // Create Rendertextures
    m_renderTargets[0].create(m_targetSize.x, m_targetSize.y);
    m_renderTargets[1].create(m_targetSize.x, m_targetSize.y);
    m_frontTarget = &m_renderTargets[0];
    m_backTarget = &m_renderTargets[1];
    m_frontTarget->clear();
    m_frontTarget->display();
    m_backTarget->clear();
    m_backTarget->display();

    // Set up the fade shader
    m_fadeShader.loadFromFile(resolvePath("data/FadeShader.frag"), sf::Shader::Fragment);
    m_fadeShader.setUniform("texture", sf::Shader::CurrentTexture);

    // Load a font and set a string as a "watermark"
    m_font.loadFromFile(resolvePath("data/LiberationSans-Regular.ttf"));

    m_authorName.setString("Maximilian Wagenbach");
    m_authorName.setFont(m_font);
    m_authorName.setCharacterSize(40);
    m_authorName.setFillColor(sf::Color(54, 54, 54));
    m_authorName.setPosition(m_targetSize.x - m_authorName.getLocalBounds().width - 10, m_targetSize.y - m_authorName.getLocalBounds().height - 10);

    // Set up some controls
    m_isFading = false;
    m_isAutoDrawing = false;
    m_isAutoSwitching = false;
    m_showWatermark = false;
    m_fadeSpeed = 0.1112;
    m_currentInterpolationType = Interpolation::Hermite;
    m_halfTargetSize = m_targetSize / 2.f;
    m_autoSwitchingDuration = sf::seconds(60);

    // Initialize the sin/cos lookup tables
    for(int i = 0; i < 8; i++)
    {
        m_sinLookupTable[i] = sin(i * PI / 4);
        m_cosLookupTable[i] = cos(i * PI / 4);
    }

    // Initialize the lines
    resetLines();
    for(int i = 0; i < 16; i++)
    {
        m_lines[i].setWidth(5);
        m_lines[i].setFillColor(sf::Color::Red);
    }

    // create an array of interpolation modes, so we can choose between them later
    m_interpolationModes[Interpolation::Hermite] = std::unique_ptr<InterpolationMode>(new HermiteInterpolation(m_targetSize));
    m_interpolationModes[Interpolation::Smoothstep] = std::unique_ptr<InterpolationMode>(new SmoothstepInterpolation(m_targetSize));
    m_interpolationModes[Interpolation::Jitter] = std::unique_ptr<InterpolationMode>(new JitterMode(m_targetSize));
    m_interpolationModes[Interpolation::Circle] = std::unique_ptr<InterpolationMode>(new CircleInterpolation(m_targetSize));
}

void CrazyPainter::update(sf::Time frameTime)
{
    m_frameTime = frameTime;

    if(m_isAutoSwitching)
        if(m_autoSwitchingClock.getElapsedTime() >= m_autoSwitchingDuration)
            changeInterpolationMode(1);

    // get the new hue shifted color
    m_color = m_colorGenerator.generateHueShiftedColor(0.25 * m_frameTime.asSeconds());

    // automatic drawing mode
    if(m_isAutoDrawing)
    {
        // get the next position from the current interpolation method
        sf::Vector2f nextPosition = m_interpolationModes[m_currentInterpolationType]->nextStep(m_frameTime);

        // the method returns a (-1, -1) vector to signal that the lines should be reset
        if(nextPosition == sf::Vector2f(-1.f, -1.f))
        {
            resetLines();
            m_interpolationModes[m_currentInterpolationType]->reset(m_lastPosition);
        }
        // otherwise the new line positions are calculated
        else
        {
            calculateLines(nextPosition);
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

void CrazyPainter::render(sf::RenderTarget& target)
{
    // Draw everything to the RenderTexture
    m_frontTarget->clear(sf::Color::Transparent);
    if(m_isFading)
    {
        m_fadeShader.setUniform("fRate", m_frameTime.asSeconds() * m_fadeSpeed);
        m_frontTarget->draw(sf::Sprite(m_backTarget->getTexture()), &m_fadeShader);
    }
    else
        m_frontTarget->draw(sf::Sprite(m_backTarget->getTexture()));

    for(int i = 0; i < 16; i++)
    {
        m_frontTarget->draw(m_lines[i]);
    }
    m_frontTarget->display();
    std::swap(m_frontTarget, m_backTarget);



    // Draw to the window
    m_renderSprite.setTexture(m_backTarget->getTexture());
    target.draw(m_renderSprite);
    if(m_showWatermark)
        target.draw(m_authorName);
}

void CrazyPainter::handleEvents(sf::Event& event, sf::Window& window)
{
    // Key Events
    if(event.type == sf::Event::KeyReleased)
    {
        switch(event.key.code)
        {
        case sf::Keyboard::C:
            // Clear RenderTexture
            m_backTarget->clear();
            m_backTarget->display();
            break;
        case sf::Keyboard::F:
            // Toggle RenderTexture fading
            m_isFading = !m_isFading;
            break;
        case sf::Keyboard::W:
            // Toggle Watermark
            m_showWatermark = !m_showWatermark;
            break;
        case sf::Keyboard::A:
            // Toggle Automatic drawing Mode
            m_isAutoDrawing = !m_isAutoDrawing;
            window.setMouseCursorVisible(!m_isAutoDrawing);
            m_interpolationModes[m_currentInterpolationType]->reset(m_lastPosition);
            resetLines();
            break;
        case sf::Keyboard::S:
            // toggle automatic switching between interpolation modes
            m_autoSwitchingClock.restart();
            m_isAutoSwitching = !m_isAutoSwitching;
            break;
        case sf::Keyboard::Right:
            // advance to the next interpolation mode
            changeInterpolationMode(1);
            m_interpolationModes[m_currentInterpolationType]->reset(m_lastPosition);
            break;
        case sf::Keyboard::Left:
            // go to the previous interpolation mode
            changeInterpolationMode(-1);
            m_interpolationModes[m_currentInterpolationType]->reset(m_lastPosition);
            break;
        // Switch between interpolation modes
        case sf::Keyboard::Num1:
            m_currentInterpolationType = Interpolation::Hermite;
            m_interpolationModes[m_currentInterpolationType]->reset(m_lastPosition);
            break;
        case sf::Keyboard::Num2:
            m_currentInterpolationType = Interpolation::Smoothstep;
            m_interpolationModes[m_currentInterpolationType]->reset(m_lastPosition);
            break;
        case sf::Keyboard::Num3:
            resetLines();
            m_currentInterpolationType = Interpolation::Jitter;
            m_interpolationModes[m_currentInterpolationType]->reset(m_lastPosition);
            break;
        case sf::Keyboard::Num4:
            m_currentInterpolationType = Interpolation::Circle;
            m_interpolationModes[m_currentInterpolationType]->reset(m_lastPosition);
            break;
        default:
            break;
        }
    }
    // in manual mode reset the lines on MouseRelease
    else if (!m_isAutoDrawing && (event.type == sf::Event::MouseButtonReleased) && (event.mouseButton.button == sf::Mouse::Left))
        resetLines();
}

void CrazyPainter::changeInterpolationMode(int step)
{
    m_autoSwitchingClock.restart();

    if(step == 1)
        ++m_currentInterpolationType;
    else if(step == -1)
        --m_currentInterpolationType;

    if(m_isAutoSwitching)
    {
        if(m_currentInterpolationType == Interpolation::Jitter)
        {
            m_isFading = false;
            resetLines();
            m_backTarget->clear();
            m_backTarget->display();
        }
        else
        {
            m_isFading = true;

            // choose randomly between slow and fast fading
            if(randomBool() == true)
                m_fadeSpeed = 0.1112; // slow fading
            else
                m_fadeSpeed = 0.12; // fast fading
        }

    m_interpolationModes[m_currentInterpolationType]->reset(m_lastPosition);

    }
}

void CrazyPainter::resetLines()
{
    for(int i = 0; i < 16; i++)
    {
        m_lines[i].setStartPoint(m_halfTargetSize);
        m_lines[i].setEndPoint(m_halfTargetSize);
    }

    m_lastPosition = m_halfTargetSize;
}

void CrazyPainter::calculateLines(const sf::Vector2f nextPoint)
{
    const float xDiff = nextPoint.x - m_halfTargetSize.x;
    const float yDiff = nextPoint.y - m_halfTargetSize.y;

    for(int i = 0; i < 8; i++)
    {
        const float cosXDiff = m_cosLookupTable[i] * xDiff;
        const float cosYDiff = m_cosLookupTable[i] * yDiff;
        const float sinXDiff = m_sinLookupTable[i] * xDiff;
        const float sinYDiff = m_sinLookupTable[i] * yDiff;

        m_lines[i].setStartPoint(m_lines[i].getEndPoint());
        m_lines[i].setEndPoint(sf::Vector2f(cosXDiff - sinYDiff + m_halfTargetSize.x, sinXDiff + cosYDiff + m_halfTargetSize.y));
        m_lines[i].setFillColor(m_color);

        m_lines[i + 8].setStartPoint(m_lines[i + 8].getEndPoint());
        m_lines[i + 8].setEndPoint(sf::Vector2f(cosXDiff * - 1 - sinYDiff + m_halfTargetSize.x, sinXDiff * - 1 + cosYDiff + m_halfTargetSize.y));
        m_lines[i + 8].setFillColor(m_color);
    }

    m_lastPosition = nextPoint;
}
