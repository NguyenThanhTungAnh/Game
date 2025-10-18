#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>   
#include "MainMenu.h"
#include "Game.h"  
#include "Globals.h"
#include <iostream>

using namespace sf;

int main()
{

    sf::RenderWindow win(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Flappy Bird", sf::Style::Close | sf::Style::Titlebar);

    sf::Image icon;
    if (icon.loadFromFile("assets/birdup.png"))
    {
        win.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    }

    MainMenu mainMenu( WIN_WIDTH, WIN_HEIGHT);
    Game game(win);

    // Nhạc nền
    sf::Music bgMusic;
    if (bgMusic.openFromFile("assets/bgm.ogg"))
    {
        bgMusic.setLoop(true);
        bgMusic.setVolume(35.f);
        bgMusic.play();
    }


    // MAIN MENU
    enum class State { MENU, GAME, EXIT };
    State state = State::MENU;

    while (win.isOpen())
    {
        Event event;
        while (win.pollEvent(event))
        {
            if (event.type == Event::Closed)
                win.close();

            if (state == State::MENU)
            {
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
                {
                    Vector2f mousePos = win.mapPixelToCoords(Mouse::getPosition(win));
                    int button = mainMenu.handleClick(mousePos);

                    if (button == 0)
                        state = State::GAME;
                    else if (button == 1)
                        state = State::EXIT;
                }
            }
        }

        win.clear(Color::Cyan);

        if (state == State::MENU)
        {
            mainMenu.draw(win);
        }

        else if (state == State::GAME)
        {
            game.startGameLoop();
            state = State::MENU;
        }

        else if (state == State::EXIT)
        {
            win.close();
        }

        win.display();
    }

    return 0;
}
