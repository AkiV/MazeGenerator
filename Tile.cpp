#include "Tile.h"

void Tile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(rectangle);
}

// Updates the color of the tile to correspond to the type.
void Tile::UpdateColor()
{
	switch (type)
	{
		case TileType::Path:
			rectangle.setFillColor(sf::Color::White);
			rectangle.setOutlineColor(sf::Color::Green);
			break;
		case TileType::Wall:
			rectangle.setFillColor(sf::Color::Green);
			rectangle.setOutlineColor(sf::Color::White);
			break;
		default:
			break;
	}
}

sf::Vector2f Tile::GetSize() const
{
	return rectangle.getSize();
}

TileType Tile::GetType() const
{
	return type;
}

sf::Vector2f Tile::GetCenter() const
{
	sf::Vector2f worldPosition = rectangle.getPosition();

	return { worldPosition.x + rectangle.getSize().x * 0.5f,
		     worldPosition.y + rectangle.getSize().y * 0.5f };
}

void Tile::SetType(TileType type)
{
	this->type = type;
	UpdateColor();
}

void Tile::SetColor(sf::Color color)
{
	rectangle.setFillColor(color);
	rectangle.setOutlineColor(color);
}

void Tile::SetPosition(sf::Vector2i gridPosition)
{
	sf::Vector2f position = static_cast<sf::Vector2f>(gridPosition);
	position.x *= rectangle.getSize().x;
	position.y *= rectangle.getSize().y;

	rectangle.setPosition(position);
}