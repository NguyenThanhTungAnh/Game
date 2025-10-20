#include "Game.h"
#include "Globals.h"
#include <iostream>
#include <sstream>
#include <SFML/Audio.hpp>
#include<fstream>
Game::Game(sf::RenderWindow& window) : win(window), 
is_space_pressed(false), 
run_game(true),
start_monitoring(false),
pipe_counter(71),
pipe_spawn_time(70),
score(0)
{
	win.setFramerateLimit(60);
	bg_texture.loadFromFile("assets/bg.png");
	bg_sprite.setTexture(bg_texture);
	bg_sprite.setScale(SCALE_FACTOR, SCALE_FACTOR);
	bg_sprite.setPosition(0.f, -250.f);

	ground_texture.loadFromFile("assets/ground.png");
	ground_sprite1.setTexture(ground_texture);
	ground_sprite2.setTexture(ground_texture);

	ground_sprite1.setScale(SCALE_FACTOR, SCALE_FACTOR);
	ground_sprite2.setScale(SCALE_FACTOR, SCALE_FACTOR);

	ground_sprite1.setPosition(0, 578);
	ground_sprite2.setPosition(ground_sprite1.getGlobalBounds().width,578);

	score_best_texture.loadFromFile("assets/score_best.png");
	score_best_sprite.setTexture(score_best_texture);
	score_best_sprite.setScale(SCALE_FACTOR* 0.6, SCALE_FACTOR* 0.6);
	score_best_sprite.setPosition(222, 200);

	restart_texture.loadFromFile("assets/restart.png");
	restart_sprite.setTexture(restart_texture);
	restart_sprite.setScale(SCALE_FACTOR*0.4 , SCALE_FACTOR *0.4);
	restart_sprite.setPosition(222,420);

	game_over_texture.loadFromFile("assets/game_over.png");
	game_over_sprite.setTexture(game_over_texture);
	game_over_sprite.setScale(SCALE_FACTOR*0.6, SCALE_FACTOR*0.6);
	game_over_sprite.setPosition(-1, -25);

	font.loadFromFile("assets/PS2T.ttf");

	start_text.setFont(font);
	start_text.setCharacterSize(20);
	start_text.setFillColor(sf::Color::Black);
	start_text.setPosition(100, 300);
	start_text.setString("Press Space to Start");
	
	score_text.setFont(font);
	score_text.setCharacterSize(20);
	score_text.setFillColor(sf::Color::Black);
	score_text.setPosition(15, 15);
	score_text.setString("Score: 0");

	
	currentScore_Text.setFont(font);
	currentScore_Text.setCharacterSize(23);
	currentScore_Text.setFillColor(sf::Color(84, 56, 71));
	currentScore_Text.setPosition(290, 269);
	currentScore_Text.setString(std::to_string(score));
	

	highestScoreText.setFont(font);
	highestScoreText.setCharacterSize(23);
	highestScoreText.setFillColor(sf::Color(84, 56, 71));
	highestScoreText.setPosition(290, 279);
	highestScoreText.setString("");


	wingBuffer.loadFromFile("assets/sfx/flap.wav");
	pointBuffer.loadFromFile("assets/sfx/score.wav");
	hitBuffer.loadFromFile("assets/sfx/dead.wav");
	wingSound.setBuffer(wingBuffer);
	wingSound.setVolume(30.0f);
	pointSound.setBuffer(pointBuffer);
	pointSound.setVolume(30.0f);
	hitSound.setBuffer(hitBuffer);

	Pipe::loadTextures();
	highScore = LoadHighScore();
}

void Game::doProcessing(sf::Time& dt)
{
	if (run_game && !is_space_pressed) 
{
	if(blickClock.getElapsedTime().asSeconds() >= Blink_rate)
	{
		isStartTextVisible = !isStartTextVisible; // Đảo trạng thái hiển thị
		blickClock.restart(); // Đặt lại đồng hồ
	}
}
	if (is_space_pressed)
	{
		moveGround(dt);

		if (pipe_counter > pipe_spawn_time)
		{
			pipes.push_back(Pipe(dist(rd)));
			pipe_counter = 0;
		}
		pipe_counter++;

		for (int i = 0;i < pipes.size();i++)
		{
			pipes[i].update(dt);
			if (pipes[i].getRightBound() < 0)
			{
				pipes.erase(pipes.begin() + i);
			}
		}

		checkCollisions();
		checkScore();
	}
	bird.update(dt);
}

void Game::startGameLoop() 
{
	sf::Clock clock;

	while (win.isOpen()) 
	{
		sf::Time dt = clock.restart();
		sf::Event event;
		while (win.pollEvent(event)) 
		{
			if (event.type == sf::Event::Closed)
			{
				win.close();
			}
			if (event.type == sf::Event::KeyPressed && run_game)
			{
				if (event.key.code == sf::Keyboard::Space && !is_space_pressed)
				{
					is_space_pressed = true;
					bird.setShouldFly(true);
					wingSound.play();
				}
				if (event.key.code == sf::Keyboard::Space && is_space_pressed)
				{
					bird.flapBird(dt);
					wingSound.play();
				}
			}
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && !run_game)
			{
				if (restart_sprite.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
				{
					restartGame();
				}
			}
		}

		doProcessing(dt);
		
		draw();
		win.display();
	}
}

void Game::checkCollisions()
{
	if (pipes.size() > 0) 
	{
		if (pipes[0].sprite_down.getGlobalBounds().intersects(bird.bird_sprite.getGlobalBounds()) ||
			pipes[0].sprite_up.getGlobalBounds().intersects(bird.bird_sprite.getGlobalBounds()) ||
			bird.bird_sprite.getGlobalBounds().top >= 548)
		{
			is_space_pressed = false;
			run_game = false;
			hitSound.play();
			if (score > highScore) {
				SaveHighScore();      
				highScore = score;    // CẬP NHẬT biến thành viên high score
			}

			// 2. Cập nhật hiển thị (chỉ một lần) khi trò chơi kết thúc
			UpdateHighScore();
		}
	}
}

void Game::checkScore()
{
	if (pipes.size() > 0)
	{
		if (!start_monitoring)
		{
			if (bird.bird_sprite.getGlobalBounds().left > pipes[0].sprite_down.getGlobalBounds().left &&
				bird.getRightBound() < pipes[0].getRightBound())
			{
				start_monitoring = true;
			}
		}
		else
		{
			if (bird.bird_sprite.getGlobalBounds().left > pipes[0].getRightBound())
			{
				score++;
				score_text.setString("Score: " + toString(score));
				pointSound.play();
				start_monitoring = false;
			}
		}
	}
}

void Game::draw()
{
	win.draw(bg_sprite);
	for (Pipe& pipe : pipes) 
	{
		win.draw(pipe.sprite_down);
		win.draw(pipe.sprite_up);
	}
	win.draw(ground_sprite1);
	win.draw(ground_sprite2);
	win.draw(bird.bird_sprite);
	if (is_space_pressed)
	{
		win.draw(score_text);
	}
	if (run_game && !is_space_pressed && isStartTextVisible)
	{
	win.draw(start_text);
	}

	if (!run_game)
	{
		win.draw(score_best_sprite);
		win.draw(currentScore_Text);
		win.draw(highestScoreText);
		win.draw(restart_sprite);
		win.draw(game_over_sprite);
		
	}
}

void Game::moveGround(sf::Time& dt)
{
	ground_sprite1.move(-move_speed * dt.asSeconds(), 0.f);
	ground_sprite2.move(-move_speed * dt.asSeconds(), 0.f);

	if (ground_sprite1.getGlobalBounds().left + ground_sprite1.getGlobalBounds().width < 0)
	{
		ground_sprite1.setPosition(ground_sprite2.getGlobalBounds().left + ground_sprite2.getGlobalBounds().width, 578);
	}
	if (ground_sprite2.getGlobalBounds().left + ground_sprite2.getGlobalBounds().width < 0)
	{
		ground_sprite2.setPosition(ground_sprite1.getGlobalBounds().left + ground_sprite1.getGlobalBounds().width, 578);
	}

}

void Game::restartGame()
{
	bird.resetBirdPosition();
	bird.setShouldFly(false);
	run_game = true;
	is_space_pressed = false;
	pipe_counter = 71;
	pipes.clear();
	score=0;
	score_text.setString("Score: 0");
	
}

std::string Game::toString(int num)
{
	std::stringstream ss;
	ss << num;
	return ss.str();
}

int Game::LoadHighScore()
{
	std::ifstream file(HIGHSCORE_FILE);
	int loadedScore = 0;// biến tạm để lưu điểm đọc được
	//ngoại lệ 1: xử lý lỗi không mở được file
	if (file.is_open())
	{
		file >> loadedScore;//Đọc dữ liệu từ file
		if(file.fail())//ngoại lệ 2:Xử lý lỗi đọc dữ liệu ví dụ như file chứa chữ thay vì số
		{
		std::cerr <<"File highscore.txt bi hong hoac khong hop le. Diem duoc thiet lap ve 0" << std::endl;
			loadedScore=0;
			//Xóa cờ lỗi của stream để tránh các lỗi khác 
			file.clear();
		}
		file.close();
	}
	// nếu file ko mở được thì loadedscore vẫn là 0
	return loadedScore;
}
void Game::SaveHighScore()
{
	if (score > highScore)
	{
		std::ofstream file(HIGHSCORE_FILE);
		if (file.is_open())
		{
			file << score;
			file.close();
		}
	}
}
void Game::UpdateHighScore()
{
	currentScore_Text.setString(toString(score));
	sf::FloatRect textRect1 = currentScore_Text.getLocalBounds();
	currentScore_Text.setOrigin(textRect1.width / 2.0f, textRect1.top);
	currentScore_Text.setPosition(300.0f, 269.0f);

	highestScoreText.setString(toString(highScore));
	sf::FloatRect textRect2 = highestScoreText.getLocalBounds();
	highestScoreText.setOrigin(textRect2.width / 2.0f, textRect2.top);
	highestScoreText.setPosition(300.0f, 345.0f);
}
