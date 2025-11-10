#pragma once
#include<SFML/Graphics.hpp>
#include "Bird.h"
#include<vector>
#include "Pipe.h"
#include<random>
#include<SFML/Audio.hpp>
#include<fstream>
#include "ChooseDifficulty.h"
#include "MainMenu.h"
#include <algorithm>

enum GameState {
	MENU,
	CHOOSE_DIFFICULTY, // <--- Trạng thái mới
	PLAYING,
	GAME_OVER,
	EXIT
};
class Game
{
public:
	Game(sf::RenderWindow& window);
	sf::RenderWindow& win;
	void startGameLoop();
	void run();
	void resetGame(Difficulty diff);
	int generateNextPipeY();
private:
	sf::Texture bg_texture, ground_texture, score_best_texture, restart_texture, game_over_texture, mainmenu_texture;
	sf::Sprite bg_sprite, ground_sprite1, ground_sprite2, score_best_sprite, restart_sprite, game_over_sprite, mainmenu_sprite;
	sf::SoundBuffer wingBuffer;
	sf::SoundBuffer pointBuffer;
	sf::SoundBuffer hitBuffer;
	sf::Sound wingSound;
	sf::Sound pointSound;
	sf::Sound hitSound;
	sf::Clock blickClock;
	bool isStartTextVisible = true;// trạng thái hiển thị của start_text
	const float Blink_rate = 0.5f;// tuần suất 0.5s/lần
	Bird bird;
	bool is_space_pressed, run_game,start_monitoring;
	int move_speed = 270;
	int lastpipe_y;
	int max_step_y = 20;
	const int pipe_min_y = 250,pipe_max_y=550;
	void draw();
	void moveGround(sf::Time&);
	void doProcessing(sf::Time& dt);
	void checkCollisions();
	void restartGame();
	void checkScore();
	void HandleGameOver();
	std::string toString(int);
	int pipe_counter, pipe_spawn_time,score, highScore;
	void UpdateHighScore();
	void SaveHighScore();
	std::vector<Pipe> pipes;
	std::random_device rd;
	sf::Font font;
	sf::Text score_text,currentScore_Text, start_text;
	sf::Text highestScoreText;
	const std::string HIGHSCORE_FILE = "highestscore.txt";
	int LoadHighScore();
	MainMenu mainMenu;
	ChooseDifficulty chooseDifficulty; // <--- Khai báo đối tượng
	GameState gameState;
};

