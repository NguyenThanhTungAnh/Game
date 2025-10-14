#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class MainMenu
{
private:
    Texture startTexture, scoreTexture, exitTexture, bgTexture, groundTexture, titleTexture;
    Sprite startButton, scoreButton, exitButton, bgSprite, groundSprite, titleSprite;

public:
    MainMenu(float width, float height);

    void draw(RenderWindow& window);
    int handleClick(Vector2f mousePos); // 0 = Start, 1 = Score, 2 = Exit, -1 = Ko co chi
};
