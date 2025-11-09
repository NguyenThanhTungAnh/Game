#pragma once
#include<SFML/Graphics.hpp>
class Pipe
{
public:
	Pipe(int startY);
	sf::Sprite sprite_up, sprite_down;
	static void loadTextures();
	void update(sf::Time&);
	float getRightBound();
	static int move_speed;
	static int pipe_distance;
	bool isPassed;
private:
	static sf::Texture texture_down, texture_up;
};

