#ifndef MAZE_H
#define MAZE_H

#include "Tile.h"
#include "Matrix.h"
#include <stack>

class Maze : public sf::Drawable
{
	public:
		Maze(Matrix<Tile>& tiles, 
			std::vector<sf::Vertex>& wallVertices, 
			sf::Vector2i startPosition) : tiles(tiles), 
			                              wallVertices(wallVertices),
						      startPosition(startPosition) { }

		bool HasTileAt(int gridX, int gridY, TileType type);
		sf::Vector2i GetStartPosition() const { return startPosition; }
		void Solve();
		void Traverse();
		bool FindUnvisitedTile(sf::Vector2i currentPosition);
		bool FindExit(sf::Vector2i currentPosition, sf::Vector2i& outExit);

	private:
		Matrix<Tile> tiles;
		Matrix<bool> visitedTiles;
		std::stack<sf::Vector2i> tileStack;
		std::vector<sf::Vertex> wallVertices;
		sf::Vector2i startPosition;

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif
