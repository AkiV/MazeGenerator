#ifndef TILE_H
#define TILE_H

#include <SFML\Graphics.hpp>

enum class TileType { Path, Wall, Exit };

class Tile : public sf::Drawable
{
	public:
		Tile() { }
		Tile(TileType type, sf::RectangleShape& rectangle) : rectangle(rectangle) 
		{ 
			SetType(type);
		}

		void Draw(sf::RenderWindow& window);

		void SetRectangle(sf::RectangleShape& rectangle) 
		{ 
			this->rectangle = rectangle; 
		}

		void operator=(const Tile& other)
		{
			rectangle = other.rectangle;
			type = other.type;
		}
		
		void UpdateColor();

		sf::Vector2f GetSize() const;
		TileType GetType() const;
		sf::Vector2f GetCenter() const;

		void SetType(TileType type);
		void SetColor(sf::Color color);
		void SetPosition(sf::Vector2i gridPosition);

	private:
		sf::RectangleShape rectangle;
		TileType type;
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif