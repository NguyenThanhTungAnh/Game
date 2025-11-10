#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;
enum class MenuAction {
    None,   
    Play,  
    Exit
};
class MainMenu
{
private:
    Texture startTexture, scoreTexture, exitTexture, bgTexture, groundTexture, titleTexture;
    Sprite startButton, scoreButton, exitButton, bgSprite, groundSprite, titleSprite;

public:
    MainMenu(float width, float height);
    MenuAction handleInput(sf::RenderWindow& window, sf::Event& event);
    void draw(RenderWindow& window);
    int handleClick(Vector2f mousePos); // 0 = Start, 1 = Score, 2 = Exit, -1 = Ko co chi
};
