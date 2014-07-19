#ifndef CRAZYPAINTER_INCLUDE
#define CRAZYPAINTER_INCLUDE

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#ifndef PI
    #define PI 3.1415926535897932384626433832795f
#endif


#include "RoundendedLine.hpp"
#include "ColorGenerator.hpp"


namespace Interpolation
{
    enum InterpolationType
    {
        Hermite,
        Smoothstep,
        Jitter,

        InterpolationModeCount = 3
    };
}

inline Interpolation::InterpolationType& operator++(Interpolation::InterpolationType& obj)
{
    int i = obj;
    if( ++i >= Interpolation::InterpolationModeCount )
        i = Interpolation::Hermite;
    return obj = static_cast<Interpolation::InterpolationType>(i);
}

inline Interpolation::InterpolationType& operator--(Interpolation::InterpolationType& obj)
{
    int i = obj;
    if( --i <= -1 )
        i = Interpolation::InterpolationModeCount - 1;
    return obj = static_cast<Interpolation::InterpolationType>(i);
}

class CrazyPainter
{
public:
    CrazyPainter(sf::RenderWindow& window);

    void update(sf::Time FrameTime);
    void render(sf::RenderTarget& Target);
    void handleEvents(sf::Event& event, sf::Window& window);

private:
    void resetLines();
    void calculateLines(const sf::Vector2f nextPoint);
    void changeInterpolationMode(int step);

    sf::Vector2f m_TargetSize;
    sf::RenderWindow* m_window;
    sf::RenderTexture m_Targets[2];
    sf::RenderTexture* m_FrontTarget;
    sf::RenderTexture* m_BackTarget;
    sf::Sprite m_RenderSprite;
    sf::Shader m_FadeShader;
    sf::Font m_Font;
    sf::Text m_AuthorName;
    bool m_bFade;
    bool m_bAutoDrawing;
    bool m_bAutoSwitching;
    bool m_bWatermark;
    float m_fFadeSpeed;
    sf::Clock m_AutoSwitchingClock;
    sf::Time m_AutoSwitchingDuration;
    Interpolation::InterpolationType m_InterpolationType;
    sf::Vector2f m_HalfTargetSize;
    float Sin_Lookup [8];
    float Cos_Lookup [8];
    sf::Time m_FrameTime;
    RoundendedLine m_Lines [16];
    sf::Vector2f m_OldPosition;
    sf::Vector2f m_StartPosition;
    sf::Vector2f m_EndPosition;
    sf::Vector2f m_NextPosition;
    sf::Vector2f m_StartTangent;
    sf::Vector2f m_EndTangent;
    sf::Time m_TotalTime;
    sf::Clock m_CurrentTime;
    ColorGenerator m_colorGenerator;
    sf::Color m_Color;
};

#endif // CRAZYPAINTER_INCLUDE
