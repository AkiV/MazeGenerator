#include "Maze.h"
#include <SFML/OpenGL.hpp>

enum class Direction { Up, Down, Left, Right };

void Maze::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int x = 0; x < tiles.GetWidth(); x++)
	{
		for (int y = 0; y < tiles.GetHeight(); y++)
		{
			target.draw(tiles[x][y]);
		}
	}

	glLineWidth(4);

	if (wallVertices.size() > 0) {
		target.draw(&wallVertices[0], wallVertices.size(), sf::Lines);
	}
}

bool Maze::HasTileAt(int gridX, int gridY, TileType type)
{
	if ((gridX < 0 || gridX >= tiles.GetWidth()) ||
		(gridY < 0 || gridY >= tiles.GetHeight()))
		return false;

	return (tiles[gridX][gridY].GetType() == type);
}

// The basic idea of the solver is to find a path from start to exit.
// It utilizes a walker approach, basically it traverses the maze
// sequentially along the path until it reaches the exit or a dead end.
// In the case of a dead end it uses a stack to backtrack to a position
// where it can continue further. 
//
// The downside of this approach is that the solver forms unneccessary 
// loops sometimes. To avoid this, a distance heuristic could be used.
// There are also much more efficient methods, but this has been chosen 
// because of a rather intuitive implementation.
void Maze::Solve()
{
	// A matrix is created to see if tiles have been visited.
	visitedTiles = Matrix<bool>(tiles.GetWidth(), tiles.GetHeight());

	for (int x = 0; x < tiles.GetWidth(); x++) { 
		for (int y = 0; y < tiles.GetHeight(); y++) {
			visitedTiles[x][y] = false;
		}
	}

	Traverse();
	
	while (!tileStack.empty())
	{
		sf::Vector2i pos = tileStack.top();
		tiles[pos.x][pos.y].SetColor(sf::Color::Red);
		tileStack.pop();
	}
}

// Uses a stack to traverse through the Maze and visit non-wall tiles.
// The tiles that remain on the stack form the path from start to finish.
void Maze::Traverse()
{
	tileStack.push(startPosition);
	sf::Vector2i currentPosition = startPosition;

	while (true)
	{
		int& x = currentPosition.x;
		int& y = currentPosition.y;

		bool up = visitedTiles[x][y - 1];
		bool down = visitedTiles[x][y + 1];
		bool left = visitedTiles[x - 1][y];
		bool right = visitedTiles[x + 1][y];

		bool canVisitUp = !up && tiles[x][y - 1].GetType() != TileType::Wall;
		bool canVisitRight = !right && tiles[x + 1][y].GetType() != TileType::Wall;
		bool canVisitDown = !down && tiles[x][y + 1].GetType() != TileType::Wall;
		bool canVisitLeft = !left && tiles[x - 1][y].GetType() != TileType::Wall;

		if (canVisitUp)
		{
			visitedTiles[x][y] = true;
			tileStack.push({ x, y - 1 });
			currentPosition.y -= 1;
		}
		else if (canVisitRight)
		{
			visitedTiles[x][y] = true;
			tileStack.push({ x + 1, y });
			currentPosition.x += 1;
		}
		else if (canVisitDown)
		{
			visitedTiles[x][y] = true;
			tileStack.push({ x, y + 1 });
			currentPosition.y += 1;
		}
		else if (canVisitLeft)
		{
			visitedTiles[x][y] = true;
			tileStack.push({ x - 1, y });
			currentPosition.x -= 1;
		}
		else if (!tileStack.empty())
		{
			// Here we reach a dead end! Backtracking must be applied.
			// Keep popping until a neighbouring path tile or exit is found.
			while (!FindUnvisitedTile(currentPosition))
			{
				currentPosition = tileStack.top();
				tileStack.pop();
			}
		}

		sf::Vector2i exitPos;

		// To make sure that the solver does not make stupid moves
		// by running right by the exit; on every cycle we check if 
		// there is an exit on any of the adjacent tiles.
		if (FindExit(currentPosition, exitPos))
		{
			tileStack.push(exitPos);
			return;
		}
	}
}

// Checks if a tile has any unvisited tiles as neighbours.
bool Maze::FindUnvisitedTile(sf::Vector2i currentPosition)
{
	int& x = currentPosition.x;
	int& y = currentPosition.y;

	auto up = visitedTiles[x][y - 1];
	auto down = visitedTiles[x][y + 1];
	auto left = visitedTiles[x - 1][y];
	auto right = visitedTiles[x + 1][y];

	up = (!up && tiles[x][y - 1].GetType() != TileType::Wall);
	right = (!right && tiles[x + 1][y].GetType() != TileType::Wall);
	down = (!down && tiles[x][y + 1].GetType() != TileType::Wall);
	left = (!left && tiles[x - 1][y].GetType() != TileType::Wall);

	return ((up || down || left || right));
}

// Tries to find the exit from adjacent tiles.
bool Maze::FindExit(sf::Vector2i currentPosition, sf::Vector2i& outExit)
{
	int& x = currentPosition.x;
	int& y = currentPosition.y;

	if (tiles[x][y].GetType() == TileType::Exit)
		return true;

	bool up = (tiles[x][y - 1].GetType() == TileType::Exit);
	bool right = (tiles[x + 1][y].GetType() == TileType::Exit);
	bool down = (tiles[x][y + 1].GetType() == TileType::Exit);
	bool left = (tiles[x - 1][y].GetType() == TileType::Exit);

	if (up)
		currentPosition.y -= 1;
	else if (down) 
		currentPosition.y += 1;
	else if (left) 
		currentPosition.x -= 1;
	else if (right) 
		currentPosition.x += 1;

	outExit = currentPosition;

	return ((up || down || left || right));
}