#pragma once
#include <SFML/Graphics.hpp>
enum class Difficulty {
    None,
    Easy,
    Hard
};
class ChooseDifficulty
{
private:
    sf::Text titleText;
    sf::Texture easybutton_texture,hardbutton_texture, bg_texture, ground_texture;
    sf::Sprite easybutton_sprite, hardbutton_sprite, bg_sprite, ground_sprite;
    sf::Font font; // Hoặc bạn có thể dùng chung font với Game nếu muốn tiết kiệm bộ nhớ

public:
    ChooseDifficulty();
    void init(float width, float height); // Khởi tạo vị trí dựa trên kích thước cửa sổ
    void draw(sf::RenderWindow& window);
    Difficulty handleInput(sf::RenderWindow& window, sf::Event& event);
};

