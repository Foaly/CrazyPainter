#ifndef CRAZYPAINTER_INCLUDE
#define CRAZYPAINTER_INCLUDE

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <memory>

#ifndef PI
    #define PI 3.1415926535897932384626433832795f
#endif


#include "RoundendedLine.hpp"
#include "ColorGenerator.hpp"
#include "InterpolationMode.hpp"
#include "HermiteInterpolation.hpp"
#include "SmoothstepInterpolation.hpp"
#include "JitterMode.hpp"


class CrazyPainter
{
public:
    CrazyPainter(sf::RenderWindow& window);

    void update(sf::Time frameTime);
    void render(sf::RenderTarget& target);
    void handleEvents(sf::Event& event, sf::Window& window);

private:
    void resetLines();
    void calculateLines(const sf::Vector2f nextPoint);
    void changeInterpolationMode(int step);

    sf::Vector2f m_targetSize;
    sf::RenderWindow* m_window;
    std::array<sf::RenderTexture, 2> m_renderTargets;
    sf::RenderTexture* m_frontTarget;
    sf::RenderTexture* m_backTarget;
    sf::Sprite m_renderSprite;
    sf::Shader m_fadeShader;
    sf::Font m_font;
    sf::Text m_authorName;
    bool m_isFading;
    bool m_isAutoDrawing;
    bool m_isAutoSwitching;
    bool m_showWatermark;
    float m_fadeSpeed;
    sf::Clock m_autoSwitchingClock;
    sf::Time m_autoSwitchingDuration;
    Interpolation::InterpolationType m_currentInterpolationType;
    std::array<std::unique_ptr<InterpolationMode>, 3> m_interpolationModes;
    sf::Vector2f m_halfTargetSize;
    std::array<float, 8> m_sinLookupTable;
    std::array<float, 8> m_cosLookupTable;
    sf::Time m_frameTime;
    std::array<RoundendedLine, 16> m_lines;
    sf::Vector2f m_lastPosition;
    ColorGenerator m_colorGenerator;
    sf::Color m_color;
};

#endif // CRAZYPAINTER_INCLUDE
