#include "ChooseDifficulty.h"
#include "Globals.h"
#include <iostream>
#include "MainMenu.h"
ChooseDifficulty::ChooseDifficulty() {
    // Tải font (đảm bảo bạn có file font này hoặc thay bằng font bạn đang dùng)
    if (!font.loadFromFile("assets/PS2T.ttf")) {
        // Xử lý lỗi nếu không tải được font
    }
}

void ChooseDifficulty::init(float width, float height) {
    // Cấu hình tiêu đề
    titleText.setFont(font);
    titleText.setString("CHOOSE DIFFICULTY");
    titleText.setCharacterSize(30);
    titleText.setFillColor(sf::Color::White);
    sf::FloatRect titleRect = titleText.getLocalBounds();
    titleText.setOrigin(titleRect.left + titleRect.width / 2.0f, titleRect.top + titleRect.height / 2.0f);
    titleText.setPosition(width / 2.0f, height / 4.0f);

    // Cấu hình nút Easy
    easybutton_texture.loadFromFile("assets/easy_button.png");
    easybutton_sprite.setTexture(easybutton_texture);
	easybutton_sprite.setScale(SCALE_FACTOR*0.15, SCALE_FACTOR*0.15);
    sf::FloatRect easyRect = easybutton_sprite.getLocalBounds();
    easybutton_sprite.setOrigin(easyRect.left + easyRect.width / 2.0f, easyRect.left + easyRect.height / 2.0f);
    easybutton_sprite.setPosition(width / 2.0f, height / 2.25f);

    // Cấu hình nút Hard
	hardbutton_texture.loadFromFile("assets/hard_button.png");
	hardbutton_sprite.setTexture(hardbutton_texture);
    hardbutton_sprite.setScale(SCALE_FACTOR * 0.15, SCALE_FACTOR * 0.15);
    sf::FloatRect hardRect = hardbutton_sprite.getLocalBounds();
    hardbutton_sprite.setOrigin(hardRect.left + hardRect.width / 2.0f, hardRect.top + hardRect.height / 2.0f);
    hardbutton_sprite.setPosition(width / 2.0f, height / 1.6f);

    if (bg_texture.loadFromFile("assets/bg.png"))
    {
        bg_sprite.setTexture(bg_texture);
        bg_sprite.setScale(SCALE_FACTOR, SCALE_FACTOR);
        bg_sprite.setPosition(0.f, -250.f);
    }

    // Thêm "Ground"
    if (ground_texture.loadFromFile("assets/ground.png"))
    {
        ground_sprite.setTexture(ground_texture);
        ground_sprite.setScale(SCALE_FACTOR, SCALE_FACTOR);
        ground_sprite.setPosition(0.f, 578.f);
    }
}

void ChooseDifficulty::draw(sf::RenderWindow& window) {
    window.draw(bg_sprite);
    window.draw(ground_sprite);
    window.draw(titleText);
    window.draw(easybutton_sprite);
    window.draw(hardbutton_sprite);
}

Difficulty ChooseDifficulty::handleInput(sf::RenderWindow& window, sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

            // Kiểm tra bấm vào nút Easy
            if (easybutton_sprite.getGlobalBounds().contains(mousePosF)) {
                return Difficulty::Easy;
            }
            // Kiểm tra bấm vào nút Hard
            if (hardbutton_sprite.getGlobalBounds().contains(mousePosF)) {
                return Difficulty::Hard;
            }
        }
    }
    return Difficulty::None;
}