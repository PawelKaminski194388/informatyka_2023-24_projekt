
#include <iostream>
#include <vector>
#include <ctime>
#include <map>
#include <time.h>
#include <string>
#include <sstream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

//PLAYER
class Player
{
private:
	sf::Sprite sprite;
	sf::Texture texture;

	float movementSpeed;

	float attackCooldown;
	float attackCooldownMax;

	int hp;
	int hpmax;
	void initVariables();
	void initTexture();
	void initSprite();
public:
	Player();
	virtual ~Player();

	const sf::Vector2f& getPos() const;
	const sf::FloatRect getBounds() const;

	const int& gethp() const;
	const int& gethpmax() const;
	void setPosition(const float x, const float y);
	void sethp(const int hp);
	void losehp(const int value);
	void move(const float dirX, const float dirY);
	const bool canAttack();
	void updateAttack();
	void update();
	void render(sf::RenderTarget& target);
};
void Player::initVariables()
{
	this->movementSpeed = 2.f;
	this->attackCooldownMax = 10.f;
	this->attackCooldown = this->attackCooldownMax;

	this->hpmax = 10;
	this->hp = this->hpmax;
}
void Player::initTexture()
{
	this->texture.loadFromFile("textures/ship.png");
}

void Player::initSprite()
{
	this->sprite.setTexture(this->texture);

	this->sprite.scale(0.02f, 0.02f);
}

Player::Player()
{
	this->initVariables();
	this->initTexture();
	this->initSprite();
}

Player::~Player()
{
}

const sf::Vector2f& Player::getPos() const
{
	return this->sprite.getPosition();
}

const sf::FloatRect Player::getBounds() const
{
	return this->sprite.getGlobalBounds();
}

const int& Player::gethp() const
{
	return this->hp;
}

const int& Player::gethpmax() const
{
	return this->hpmax;
}
void Player::setPosition(const float x, const float y)
{
	this->sprite.setPosition(x, y);
}

void Player::sethp(const int hp)
{
	this->hp = hp;
}

void Player::losehp(const int value)
{
	this->hp -= value;

}
void Player::move(const float dirX, const float dirY)
{
	this->sprite.move(this->movementSpeed * dirX, this->movementSpeed * dirY);
}

const bool Player::canAttack()
{
	if (this->attackCooldown >= this->attackCooldownMax)
	{
		this->attackCooldown = 0.f;
		return true;
	}
	return false;
}

void Player::updateAttack()
{
	if(this->attackCooldown<this->attackCooldownMax)
	this->attackCooldown += 0.5f;
}

void Player::update()
{
	this->updateAttack();
}

void Player::render(sf::RenderTarget& target)
{
	target.draw(this->sprite);
}



//BULLET
class Bullet
{
private:
	sf::Sprite shape;
	sf::Texture* texture;

	sf::Vector2f direction;
	float movementSpeed;
public:
	Bullet();
	Bullet(sf::Texture* texture, float pos_x, float pos_y, float dirX, float dirY, float movement_speed);
	virtual ~Bullet();

	const sf::FloatRect getBounds() const;
	void update();
	void render(sf::RenderTarget* target);
};

Bullet::Bullet()
{
}

Bullet::Bullet(sf::Texture* texture, float pos_x, float pos_y, float dirX, float dirY, float movement_speed)
{
	this->shape.setTexture(*texture);
	this->shape.setPosition(pos_x, pos_y);
	this->shape.scale(0.2f, 0.2f);
	this->direction.x = dirX;
	this->direction.y = dirY;
	this->movementSpeed = movement_speed;

}

Bullet::~Bullet()
{
}

const sf::FloatRect Bullet::getBounds() const
{
	return this->shape.getGlobalBounds();
}

void Bullet::update()
{
	this->shape.move(this->movementSpeed * this->direction);
}

void Bullet::render(sf::RenderTarget* target)
{
	target->draw(this->shape);
}

//ENEMY
class Enemy
{
private:
	unsigned pointCount;
	sf::CircleShape shape;
	int type;
	int hp;
	int hpmax;
	int damage;
	float speed;
	int points;

	void initVariables();
	void initShape();

public:
	Enemy(float pos_x, float pos_y);

	virtual ~Enemy();
	const sf::FloatRect getBounds() const;
	const int& getPoints() const;
	const int& getDamage() const;
	void update();
	void render(sf::RenderTarget* target);
};



void Enemy::initVariables()
{
	 this->pointCount = rand() % 8 + 3;
	 this->type=0;
	 this->speed = static_cast<float>(this->pointCount);
	 this->hpmax = static_cast<int>(this->pointCount);
	 this->hp = this->hpmax;
	 this->damage = this->pointCount;
	 this->points = this->pointCount;

}
void Enemy::initShape()
{
	this->shape.setRadius(this->pointCount*4);
	this->shape.setPointCount(this->pointCount);
	this->shape.setFillColor(sf::Color(rand() % 255 + 1, rand() % 255 + 1, rand() % 255 + 1, 255));
}

Enemy::Enemy(float pos_x, float pos_y)
{ 

	this->initVariables();
	this->initShape();
	this->shape.setPosition(pos_x, pos_y);
}

Enemy::~Enemy()
{
}


const sf::FloatRect Enemy::getBounds() const
{
	return this->shape.getGlobalBounds();
}

const int& Enemy::getPoints() const
{
	return this->points;
}

const int& Enemy::getDamage() const
{
	return this->damage;
}
void Enemy::update()
{
	this->shape.move(0.f, this->speed);
}

void Enemy::render(sf::RenderTarget* target)
{
	target->draw(this->shape);
}

//GAME
class Game
{

private:
	sf::RenderWindow* window;

	std::map<std::string, sf::Texture*> textures;
	std::vector<Bullet*> bullets;
	sf::Font font;
	sf::Text pointText;
	sf::Texture backgroundtexture;
	sf::Sprite background;
	unsigned points;
	Player* player;

	sf::RectangleShape playerhpbar;
	sf::RectangleShape playerhpbarback;

	float spawnTimer;
	float spawnTimerMax;
	std::vector<Enemy*> enemies;



	void initWindow();
	void initTextures();
	void initGUI();
	void initWorld();
	void initPoints();
	void initPlayer();
	void initEnemies();
public:
	Game();
	virtual ~Game();

	void run();

	void updatePollEvents();
	void updateInput();
	void updateGUI();
	void updateBackground();
	void updateCollision();
	void updateBullets();
	void updateEnemies();
	void updateCombat();
	void update();
	void renderGUI();
	void renderBackground();
	void render();
};


void Game::initWindow()
{
	
}

void Game::initWindow()
{
	this->window = new sf::RenderWindow(sf::VideoMode(800, 600), "Projekt Informatyka", sf::Style::Close | sf::Style::Titlebar);
	this->window->setFramerateLimit(144);

	this->window->setVerticalSyncEnabled(false);
}

void Game::initTextures()
{
	this->textures["BULLET"] = new sf::Texture();
	this->textures["BULLET"]->loadFromFile("textures/bullet.png");
	
}

void Game::initGUI()
{
	this->font.loadFromFile("textures/Stopbuck.ttf");

	this->pointText.setFont(this->font);
	this->pointText.setCharacterSize(20);
	this->pointText.setFillColor(sf::Color::White);
	this->pointText.setString("test");
	this->playerhpbar.setSize(sf::Vector2f(300.f, 25.f));
	this->playerhpbar.setFillColor(sf::Color::Green);
	this->playerhpbar.setPosition(sf::Vector2f(20.f, 20.f));

	this->playerhpbarback = this->playerhpbar;
	this->playerhpbarback.setFillColor(sf::Color::Black);
}

void Game::initWorld()
{
	this->backgroundtexture.loadFromFile("textures/background.jpg");
	this->background.setTexture(this->backgroundtexture);
}

void Game::initPoints()
{
	this->points = 0;
}

void Game::initPlayer()
{ 
	this->player = new Player();

}

void Game::initEnemies()
{
	this->spawnTimerMax = 50.f;
	this->spawnTimer = this->spawnTimerMax;
}

Game::Game()
{
	this->initWindow();
	this->initTextures();
	this->initGUI();
	this->initWorld();
	this->initPoints();
	this->initPlayer();
	this->initEnemies();
}

Game::~Game()
{
	delete this->window;
	delete this->player;

	for (auto& i : this->textures)
	{
		delete i.second;
	}

	for (auto* i : this->bullets)
	{
		delete i;
	}
	for (auto* i : this->enemies)
	{
		delete i;
	}
}

void Game::run()
{
	while (this->window->isOpen())
	{
		this->update();
		this->render();
	}
}

void Game::updatePollEvents()
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

void Game::updateInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		this->player->move(-1.f, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		this->player->move(1.f, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		this->player->move(0.f, -1.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		this->player->move(0.f, 1.f);

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->player->canAttack())
	{
		this->bullets.push_back(new Bullet(this->textures["BULLET"], this->player->getPos().x + this->player->getBounds().width/2.f, this->player->getPos().y, 0.f, -1.f, 5.f));
	}
}

void Game::updateGUI()
{
	std::stringstream ss;

	ss << "Punkty:" << this->points;
	this->pointText.setString(ss.str());

	this->player->losehp(5);
	float hppercent = this->player->gethp() / this->player->gethpmax();
	this->playerhpbar.setSize(sf::Vector2f(this->playerhpbar.getSize().x * hppercent, this->playerhpbar.getSize().y));

}

void Game::updateBackground()
{
}

void Game::updateCollision()
{
	if(this->player->getBounds().left < 0.f)
	{
		this->player->setPosition(0.f, this->player->getBounds().top);
	}
	else if (this->player->getBounds().left + this->player->getBounds().width >= this->window->getSize().x)
	{
		this->player->setPosition(this->window->getSize().x - this->player->getBounds().width, this->player->getBounds().top);
	}
	if (this->player->getBounds().top < 0.f)
	{
		this->player->setPosition(this->player->getBounds().left, 0.f);
	}
}

void Game::updateBullets()
{
	unsigned counter = 0;
	for (auto* bullet : this->bullets)
	{
		bullet->update();

		if (bullet->getBounds().top + bullet->getBounds().height < 0.f)
		{
			delete this->bullets.at(counter);
			this->bullets.erase(this->bullets.begin() + counter);
			--counter;
		}
		++counter;
	}
}

void Game::updateEnemies()
{
	this->spawnTimer += 0.5f;
	if (this->spawnTimer >= this->spawnTimerMax)
	{
		this->enemies.push_back(new Enemy(rand() % this->window->getSize().x -20.f, -100.f));
		this->spawnTimer = 0.f;
	}

	unsigned counter = 0;
	for (auto* enemy : this->enemies)
	{
		enemy->update();

		if (enemy->getBounds().top  > this->window->getSize().y)
		{
			delete this->enemies.at(counter);
			this->enemies.erase(this->enemies.begin() + counter);
			--counter;
		}
		else if (enemy->getBounds().intersects(this->player->getBounds()))
		{
			this->player->losehp(1);
			delete this->enemies.at(counter);
			this->enemies.erase(this->enemies.begin() + counter);
			--counter;


		}

		++counter;
	}

}

void Game::updateCombat()
{

	for (int i = 0; i < this->enemies.size(); ++i)
	{
		bool enemy_deleted = false;
		for (size_t k = 0; k < this->bullets.size() && enemy_deleted == false; k++)
		{
			if (this->enemies[i]->getBounds().intersects(this->bullets[k]->getBounds()))
			{
				this->points += this->enemies[i]->getPoints();
				delete this->enemies[i];
				this->enemies.erase(this->enemies.begin() + i);

				delete this->bullets[k];
				this->bullets.erase(this->bullets.begin() + k);

				enemy_deleted = true;
			}
		}
	}
/*	for (int i = 0; i < this->enemies.size(); ++i)
	{
		bool enemy_removed = false;
		this->enemies[i]->update();

		for (size_t k = 0; k < this->bullets.size() && !enemy_removed; k++)
		{
			if (this->bullets[k]->getBounds().intersects(this->enemies[k]->getBounds()))
			{
				this->bullets.erase(this->bullets.begin() + k);
				this->enemies.erase(this->enemies.begin() + i);
				enemy_removed = true;
			}
		}
		if (!enemy_removed)
		{
			if (this->enemies[i]->getBounds().top > this->window->getSize().y)
			{
				this->enemies.erase(this->enemies.begin() + i);
				std::cout << this->enemies.size() << "\n";
				enemy_removed = true;

			}
		}
	} */
}

void Game::update()
{
	this->updatePollEvents();
	this->updateInput();
	this->player->update();
	this->updateCollision();
	this->updateBullets();
	this->updateEnemies();
	this->updateCombat();
	this->updateGUI();
	this->updateBackground();
}

void Game::renderGUI()
{
	this->window->draw(this->pointText);
	this->window->draw(this->playerhpbar);
	this->window->draw(this->playerhpbarback);
}

void Game::renderBackground()
{
	this->window->draw(this->background);
}

void Game::render()
{
	this->window->clear();
	this->renderBackground();
	this->player->render(*this->window);

	for (auto* bullet : this->bullets)
	{
		bullet->render(this->window);
	}

	for (auto* enemy : this->enemies)
	{
		enemy->render(this->window);
	}

	this->renderGUI();
	this->window->display();
}



int main()
{
	srand(time(nullptr));
	Game game;
	game.run();
	return 0;
}


