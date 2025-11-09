#include "Game.h"
#include "Globals.h"
#include <iostream>
#include <sstream>
#include <SFML/Audio.hpp>
#include<fstream>
#include"MainMenu.h"
#include "Pipe.h"
#include "Bird.h"
Game::Game(sf::RenderWindow& window) : win(window),
is_space_pressed(false),
run_game(true),
start_monitoring(false),
pipe_counter(50),
pipe_spawn_time(70),
score(0),
mainMenu(WIN_WIDTH, WIN_HEIGHT)
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

	mainmenu_texture.loadFromFile("assets/mainmenu_button.png");
	mainmenu_sprite.setTexture(mainmenu_texture);
	mainmenu_sprite.setScale(SCALE_FACTOR * 0.147, SCALE_FACTOR * 0.147);
	mainmenu_sprite.setPosition(222, 518);

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
	lastpipe_y = (pipe_min_y + pipe_max_y) / 2;
	highScore = LoadHighScore();
	gameState = MENU;
	chooseDifficulty.init(WIN_WIDTH, WIN_HEIGHT);
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
			int newY = generateNextPipeY();
			pipes.push_back(Pipe(newY));

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

		// --- VÒNG LẶP XỬ LÝ SỰ KIỆN (INPUT) ---
		while (win.pollEvent(event))
		{
			// 1. Sự kiện Đóng cửa sổ (Luôn kiểm tra đầu tiên)
			if (event.type == sf::Event::Closed)
			{
				win.close();
			}

			// 2. Xử lý Input dựa trên TRẠNG THÁI GAME hiện tại
			switch (gameState)
			{
			case MENU:
				// MainMenu xử lý input và trả về true nếu nút Play được nhấn
				if (mainMenu.handleInput(win, event)) {
					gameState = CHOOSE_DIFFICULTY; // Chuyển sang chọn độ khó
				}
				break;

			case CHOOSE_DIFFICULTY:
			{
				// ChooseDifficulty xử lý input và trả về độ khó đã chọn
				Difficulty chosenDiff = chooseDifficulty.handleInput(win, event);
				if (chosenDiff != Difficulty::None) {
					resetGame(chosenDiff); // Áp dụng độ khó và reset game
					gameState = PLAYING;   // Bắt đầu chơi
				}
			}
			break;

			case PLAYING:
				// Xử lý input khi đang chơi (Chim nhảy)
				// Kiểm tra phím Space hoặc Click chuột để nhảy
				if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space))
				{
					if (!is_space_pressed) {
						is_space_pressed = true;
						bird.setShouldFly(true);
						wingSound.play();
					}
					// Lưu ý: Logic nhảy liên tục (giữ phím) nên xử lý ở ngoài pollEvent nếu cần
					else {
						bird.flapBird(dt);
						wingSound.play();
					}
				}
				break;

			case GAME_OVER:
				// Xử lý input khi Game Over (Restart)
				if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
				{
					// Kiểm tra click vào nút Restart
					sf::Vector2i mousePos = sf::Mouse::getPosition(win);
					if (restart_sprite.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
					{
						restartGame();
					}
					if(mainmenu_sprite.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
					{
						gameState = GameState::MENU;
					}
				}
				break;
			
			}

		} // --- KẾT THÚC VÒNG LẶP POLL EVENT ---

		// --- CẬP NHẬT LOGIC (UPDATE) ---
		// Chỉ cập nhật logic game khi đang ở trạng thái PLAYING
		if (gameState == PLAYING)
		{
			doProcessing(dt);
		}

		// --- VẼ (RENDER) ---
		win.clear();

		switch (gameState)
		{
		case MENU:
			mainMenu.draw(win);
			break;
		case CHOOSE_DIFFICULTY:
			chooseDifficulty.draw(win);
			break;
		case PLAYING:
		case GAME_OVER: // Vẽ game nền ngay cả khi Game Over
			draw(); // Hàm draw() hiện tại của bạn đã xử lý cả Playing và Game Over
			break;
		}

		win.display();
	}
}

void Game::checkCollisions()
{
	// 1. Lấy vùng bao của chim để dùng nhiều lần
	sf::FloatRect birdBounds = bird.bird_sprite.getGlobalBounds();

	// 2. Kiểm tra va chạm với MẶT ĐẤT
	if (birdBounds.top + birdBounds.height >= 578)
	{
		HandleGameOver();
		return;
	}

	// 3. Duyệt qua TẤT CẢ các ống để kiểm tra va chạm
	for (auto& pipe : pipes)
	{
		if (pipe.sprite_down.getGlobalBounds().intersects(birdBounds) ||
			pipe.sprite_up.getGlobalBounds().intersects(birdBounds))
		{
			HandleGameOver();
			return;
		}
	}
}

void Game::checkScore()
{
	for (int i = 0; i < pipes.size(); i++)
	{
		// kiểm tra 2 điều kiện:
		// 1. ông này CHƯA được tính điểm (!pipes[i].isPassed)
		// 2. cạnh trái của con chim đã vượt qua cạnh phải của ống
		if (!pipes[i].isPassed && bird.bird_sprite.getGlobalBounds().left > pipes[i].getRightBound())
		{
			pipes[i].isPassed = true; // Đánh dấu là đã đi qua để không tính điểm lại lần sau
			score++;

			// Cập nhật hiển thị và âm thanh
			score_text.setString("Score: " + std::to_string(score));
			pointSound.play();
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
		win.draw(mainmenu_sprite);
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
	pipe_counter = 50;
	pipes.clear();
	score=0;
	score_text.setString("Score: 0");
	gameState = GameState::PLAYING;
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
void Game::resetGame(Difficulty diff) {
	run_game = true;             // Đảm bảo game loop chạy lại
	is_space_pressed = false;    // Reset trạng thái chờ vỗ cánh
	score = 0;                   // Reset điểm
	score_text.setString("Score: 0");
	pipe_counter = 71;           // Reset bộ đếm sinh ống


	bird.resetBirdPosition();
	bird.setShouldFly(false); // Đảm bảo chim không tự bay khi vừa vào
	pipes.clear();

	// Áp dụng độ khó
	if (diff == Difficulty::Easy) {
		max_step_y = 250;//độ lệch tối đa giữa 2 ống liên tiếp
		bird.setFlapSpeed(250.0f); // Chim bay thấp hơn
		pipe_spawn_time = 70; 
		Pipe::move_speed = 300;   // Di chuyển nhanh hơn
	}
	else if (diff == Difficulty::Hard) {
		max_step_y = 50;
		bird.setFlapSpeed(280.0f); // Chim bay cao hơn
		pipe_spawn_time = 25; // Ống ra nhanh hơn
		Pipe::move_speed = 250;  
	}
}
void Game::HandleGameOver()
{
	// Chỉ xử lý một lần khi vừa mới thua
	if (run_game)
	{
		run_game = false;
		is_space_pressed = false;
		hitSound.play();

		// Cập nhật High Score
		if (score > highScore) {
			SaveHighScore();
			highScore = score;
		}
		UpdateHighScore();

		// Chuyển trạng thái
		gameState = GameState::GAME_OVER;
	}
}
int Game::generateNextPipeY()
{
	int current_min = lastpipe_y - max_step_y;
	int current_max = lastpipe_y + max_step_y;

	int actual_min = std::max(pipe_min_y, current_min);
	int actual_max = std::min(pipe_max_y, current_max);

	std::uniform_int_distribution<int> dist_dynamic(actual_min, actual_max);

	lastpipe_y = dist_dynamic(rd); // 'rd' là random_device của bạn

	return lastpipe_y;
}