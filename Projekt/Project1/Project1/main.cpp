#pragma once
#include <iostream>
#include <vector>
#include <ctime>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
class Player
{
private:
	sf::Sprite sprite;
	sf::Texture texture;

	void initTexture();
	void initSprite();
public:
	Player();
	virtual ~Player();

	void update();
	void render(sf::RenderTarget& target);
};

class Game
{

private:
	sf::RenderWindow* window;

	Player* player;

	void initWindow();
	void initPlayer();
public:
	Game();
	virtual ~Game();

	void run();

	void update();
	void render();
};


void Game::initWindow()
{
	this->window = new sf::RenderWindow(sf::VideoMode(800, 600), "Projekt Informatyka", sf::Style::Close | sf::Style::Titlebar);
	this->window->setFramerateLimit(144);

	this->window->setVerticalSyncEnabled(false);
}

void Game::initPlayer()
{
	this->player = new Player();
}

Game::Game()
{
	this->initWindow();
	this->initPlayer();
}

Game::~Game()
{
	delete this->window;
	delete this->player;
}

void Game::run()
{
	while (this->window->isOpen())
	{
		this->update();
		this->render();
	}
}

void Game::update()
{
	sf::Event e;
	while (this->window->pollEvent(e))
	{
		if (e.Event::type == sf::Event::Closed)
			this->window->close();

		if (e.Event::KeyPressed && e.Event::key.code == sf::Keyboard::Escape)
			this->window->close();
	}
}

void Game::render()
{
	this->window->clear();
	this->player->render(*this->window);
	this->window->display();
}


void Player::initTexture()
{
	this->texture.loadFromFile("textures/ship.png");
}

void Player::initSprite()
{
	this->sprite.setTexture(this->texture);

	this->sprite.scale(0.02f,0.02f);
}

Player::Player()
{
	this->initTexture();
	this->initSprite();
}

Player::~Player()
{
}

void Player::update()
{

}

void Player::render(sf::RenderTarget& target)
{
	target.draw(this->sprite);
}

int main()
{
	Game game;
	game.run();
	return 0;
}
