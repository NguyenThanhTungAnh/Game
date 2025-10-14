#pragma once
#include<SFML/Graphics.hpp>
#include "Bird.h"
#include<vector>
#include "Pipe.h"
#include<random>
#include<SFML/Audio.hpp>
#include<fstream>
class Game
{
public:
	Game(sf::RenderWindow& window);
	sf::RenderWindow& win;
	void startGameLoop();
private:
	sf::Texture bg_texture, ground_texture, score_best_texture, restart_texture, game_over_texture;
	sf::Sprite bg_sprite, ground_sprite1, ground_sprite2, score_best_sprite, restart_sprite, game_over_sprite;
	sf::SoundBuffer wingBuffer;
	sf::SoundBuffer pointBuffer;
	sf::SoundBuffer hitBuffer;
	sf::Sound wingSound;
	sf::Sound pointSound;
	sf::Sound hitSound;
	Bird bird;
	bool is_space_pressed, run_game,start_monitoring;
	const int move_speed = 270;
	void draw();
	void moveGround(sf::Time&);
	void doProcessing(sf::Time& dt);
	void checkCollisions();
	void restartGame();
	void checkScore();
	std::string toString(int);
	int pipe_counter, pipe_spawn_time,score, highScore;
	void UpdateHighScore();
	void SaveHighScore();
	std::vector<Pipe> pipes;
	std::random_device rd;
	std::uniform_int_distribution<int> dist{ 250,550 };
	sf::Font font;
	sf::Text score_text,currentScore_Text;
	sf::Text highestScoreText;
	const std::string HIGHSCORE_FILE = "highestscore.txt";
	int LoadHighScore();
};

