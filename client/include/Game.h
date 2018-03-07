#ifndef GAME_H
#define GAME_H


#include <SFML/Graphics.hpp>

class Game
{
public:
    Game();
    virtual ~Game();
    void					run();


protected:

private:
    void					processEvents();
    void					update(sf::Time elapsedTime);
    void					render();

    void					updateStatistics(sf::Time elapsedTime);
    void					handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
    static const float		PlayerSpeed;
    static const sf::Time	TimePerFrame;

    sf::RenderWindow		mWindow;
    sf::Texture				mTexture;
    sf::Sprite				mPlayer;
    sf::Font				mFont;
    sf::Text				mStatisticsText;
    sf::Time				mStatisticsUpdateTime;

    std::size_t				mStatisticsNumFrames;
    bool					mIsMovingUp;
    bool					mIsMovingDown;
    bool					mIsMovingRight;
    bool					mIsMovingLeft;
};

#endif // GAME_H
