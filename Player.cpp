#include "Player.h"

Player::Player(sf::Vector2f size, const std::shared_ptr<Maze>& maze)
	: maze(maze)
{
	rectangle.setSize(size);
	rectangle.setFillColor(sf::Color::Yellow);
}

void Player::UpdateWorldPosition()
{
	auto size = rectangle.getSize();
	rectangle.setPosition(X * size.x, Y * size.y);
}

void Player::SetPosition(int x, int y)
{
	this->X = x;
	this->Y = y;
	UpdateWorldPosition();
}

void Player::Update()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && 
		!maze->HasTileAt(X, Y - 1, TileType::Wall))
	{
		Move(0, -1);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && 
			!maze->HasTileAt(X, Y + 1, TileType::Wall))
	{
		Move(0, 1);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && 
		     !maze->HasTileAt(X - 1, Y, TileType::Wall))
	{
		Move(-1, 0);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && 
		     !maze->HasTileAt(X + 1, Y, TileType::Wall))
	{
		Move(1, 0);
	}
}

void Player::SetMaze(const std::shared_ptr<Maze>& maze)
{
	this->maze = maze;
}

void Player::Move(int x, int y)
{
	this->X += x;
	this->Y += y;
	UpdateWorldPosition();
}

bool Player::IsAtExit()
{
	return maze->HasTileAt(X, Y, TileType::Exit);
}

void Player::GotoStart()
{
	auto pos = maze->GetStartPosition();
	SetPosition(pos.x, pos.y);
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(rectangle);
}