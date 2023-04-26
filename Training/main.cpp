#include <SFML/Graphics.hpp>
#include <time.h>

const signed height{ 20 };
const signed width{ 10 };

signed gameField[height][width]{ 0 };

signed figures[7][4] = {
	1,3,5,7, // Figure I
	2,4,5,7, // Figure S
	3,5,4,6, // Figure Z
	3,5,4,7, // Figure T
	2,3,5,7, // Figure L
	3,5,7,6, // Figure J
	2,3,4,5, // Figure O
};

struct Point {
	signed x, y;
} a[4], b[4];

bool checkBounds() {
	for (signed i{ 0 }; i < 4; ++i) {
		if (a[i].x < 0 || a[i].x >= width || a[i].y >= height) {
			return 0;
		}

		else if (gameField[a[i].y][a[i].x]) {
			return 0;
		}
	}

	return -1;
}

signed main() {
	srand(time(0));

	sf::RenderWindow window(sf::VideoMode(320, 480), "TETRIS");

	sf::Texture texture;
	texture.loadFromFile("Images\\tiles.png");
	sf::Texture textureBackground;
	textureBackground.loadFromFile("Images\\background.png");
	sf::Texture textureFrame;
	textureFrame.loadFromFile("Images\\frame.png");

	sf::Sprite sprite(texture);
	sf::Sprite spriteBackground(textureBackground);
	sf::Sprite spriteFrame(textureFrame);

	sprite.setTextureRect(sf::IntRect(0, 0, 18, 18));

	signed colorNumber{ 1 };
	signed dx{ 0 };
	bool rotate{ 0 };
	bool beginGame{ true };
	signed number{ rand() % 7 };

	float timer{ 0 };
	float delay{ 0.3 };

	sf::Clock clock;

	while (window.isOpen()) {
		float time{ clock.getElapsedTime().asSeconds() };
		clock.restart();
		timer += time;

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}

			if (event.type == sf::Event::KeyPressed)
				if (event.key.code == sf::Keyboard::W) {
					rotate = { true };
				}

				else if (event.key.code == sf::Keyboard::A) {
					dx = -1;
				}

				else if (event.key.code == sf::Keyboard::D) {
					dx = 1;
				}
			
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			delay = { 0.05 };
		}

		for (signed i{ 0 }; i < 4; ++i) {
			b[i] = { a[i] };
			a[i].x += dx;
		}

		if (!checkBounds()) {
			for (signed i{ 0 }; i < 4; ++i) {
				a[i] = { b[i] };
			}
		}

		if (rotate) {
			Point point{ a[1] };

			for (signed i{ 0 }; i < 4; ++i) {
				signed x{ a[i].y - point.y };
				signed y{ a[i].x - point.x };
				a[i].x = { point.x - x };
				a[i].y = { point.y + y };
			}

			if (!checkBounds()) {
				for (signed i{ 0 }; i < 4; ++i) {
					a[i] = { b[i] };
				}
			}
		}

		// Downward movement of the tetramino
		if (timer > delay) {
			for (signed i{ 0 }; i < 4; ++i) {
				b[i] = a[i];
				a[i].y += 1;
			}

			if (!checkBounds()) {
				for (signed i{ 0 }; i < 4; ++i) {
					gameField[b[i].y][b[i].x] = { colorNumber };
				}

				colorNumber = { 1 + rand() % 7 };
				signed number{ rand() % 7 };

				for (signed i{ 0 }; i < 4; ++i) {
					a[i].x = figures[number][i] % 2;
					a[i].y = figures[number][i] / 2;
				}
			}

			timer = { 0 };

		}

		signed line{ height - 1 };
		for (signed i{ height - 1 }; i > 0; --i) {
			signed count{ 0 };

			for (signed j{ 0 }; j < width; ++j) {
				if (gameField[i][j]) {
					++count;
				}
				
				gameField[line][j] = { gameField[i][j] };
			}
			if (count < width) {
				--line;
			}
		}

		if (beginGame)
		{
			beginGame = { false };
			number = { rand() % 7 };
			for (signed i{ 0 }; i < 4; ++i) {
				a[i].x = { figures[number][i] % 2 };
				a[i].y = { figures[number][i] / 2 };
			}
		}
		dx = { 0 };
		rotate = { 0 };
		delay = { 0.3 };

		window.clear(sf::Color::White);
		window.draw(spriteBackground);
		for (signed i{ 0 }; i < height; ++i)
			for (signed j{ 0 }; j < width; ++j)
			{
				if (gameField[i][j] == 0) {
					continue;
				}

				sprite.setTextureRect(sf::IntRect(gameField[i][j] * 18, 0, 18, 18));
				sprite.setPosition(j * 18, i * 18);
				sprite.move(28, 31);
				window.draw(sprite);
			}

		for (signed i = { 0 }; i < 4; ++i)
		{
			sprite.setTextureRect(sf::IntRect(colorNumber * 18, 0, 18, 18));

			sprite.setPosition(a[i].x * 18, a[i].y * 18);

			sprite.move(28, 31);

			window.draw(sprite);
		}

		window.draw(spriteFrame);

		window.display();
	}

	return 0;
}