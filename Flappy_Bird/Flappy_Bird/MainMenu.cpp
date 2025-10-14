#include "MainMenu.h"
#include <iostream>
#include "Globals.h"

MainMenu::MainMenu(float width, float height)
{
    // Thêm tựa đề game
    if (titleTexture.loadFromFile("assets/flappy_bird.png"))
    {
        titleSprite.setTexture(titleTexture);
        titleSprite.setScale(0.98f, 0.98f);
        titleSprite.setPosition(-30.f, -35.f);
    }

    // Thêm nền
    if (bgTexture.loadFromFile("assets/bg.png"))
    {
        bgSprite.setTexture(bgTexture);
        bgSprite.setScale(SCALE_FACTOR, SCALE_FACTOR);
        bgSprite.setPosition(0.f, -250.f);
    }

    // Thêm "Ground"
    if (groundTexture.loadFromFile("assets/ground.png"))
    {
        groundSprite.setTexture(groundTexture);
        groundSprite.setScale(SCALE_FACTOR, SCALE_FACTOR);
        groundSprite.setPosition(0.f, 578.f);
    }

    // Thêm nút "Start"
    if (startTexture.loadFromFile("assets/start.png"))
    {
        startButton.setTexture(startTexture);
        startButton.setScale(SCALE_FACTOR, SCALE_FACTOR);
        startButton.setPosition(220.f, 300.f);
        startButton.setScale(0.45f, 0.45f);
    }

    // Thêm nút "Exit"
    if (exitTexture.loadFromFile("assets/exit.png"))
    {
        exitButton.setTexture(exitTexture);
        exitButton.setScale(SCALE_FACTOR, SCALE_FACTOR);
        exitButton.setPosition(220.f, 450.f);
        exitButton.setScale(0.45f, 0.45f);
    }

    startButton.setTexture(startTexture);
    exitButton.setTexture(exitTexture);

}

void MainMenu::draw(RenderWindow& window)
{
    if (bgTexture.getSize().x > 0)
        window.draw(bgSprite);

    // Vẽ "Ground" và "Tựa đề game"
    window.draw(groundSprite);
    window.draw(titleSprite);

    // Vẽ 3 nút
    window.draw(startButton);
    window.draw(exitButton);
}

int MainMenu::handleClick(Vector2f mousePos)
{
    if (startButton.getGlobalBounds().contains(mousePos))
        return 0;
    if (exitButton.getGlobalBounds().contains(mousePos))
        return 1;
    return -1;
}
