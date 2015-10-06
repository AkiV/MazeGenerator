#ifndef MAZE_GENERATOR_H
#define MAZE_GENERATOR_H

#include <memory>
#include "Maze.h"

enum class Direction { Up, Down, Left, Right };

class MazeGenerator
{
	public:
		std::unique_ptr<Maze> Create(int width, int height, 
			                         int resolutionWidth, int resolutionHeight);

	private:
		Matrix<Tile> tiles;
		int width {0};
		int height {0};

		void InitializeTiles(int width, int height,
			                 int resolutionWidth, int resolutionHeight);

		std::vector<sf::Vertex> CreateWalls();
		void Carve(sf::Vector2i currentPosition);
		void CreateRandomExit(sf::Vector2i startPosition);

		bool KnockWall(Direction direction, sf::Vector2i currentPosition);

		Direction GetRandomDirection();
		sf::Vector2i RandomStartPosition();	
};

#endif
