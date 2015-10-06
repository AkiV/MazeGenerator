#include "MazeGenerator.h"
#include <chrono>
#include <vector>
#include <algorithm>
#include <assert.h>

// Generates a maze randomly. Width and Height must be odd numbers
// in order to create a proper maze. Uses depth-first search. 
// For more information: http://www.migapro.com/depth-first-search/.
std::unique_ptr<Maze> 
MazeGenerator::Create(const int width, const int height, 
                      int resolutionWidth, int resolutionHeight)
{
	assert(width % 3 == 0 && height % 3 == 0);
	assert(width == height);

	InitializeTiles(width, height, resolutionWidth, resolutionHeight);

	sf::Vector2i startPosition = RandomStartPosition();
	tiles[startPosition.x][startPosition.y].SetType(TileType::Path);

	Carve(startPosition);
	CreateRandomExit(startPosition);
	auto wallVertices = CreateWalls();

	tiles[startPosition.x][startPosition.y].SetColor(sf::Color::Black);

	return std::make_unique<Maze>(tiles, wallVertices, startPosition);
}

// By default, the maze is filled with wall tiles,
// so that the path can be carved through it.
void MazeGenerator::InitializeTiles(int width, int height,
				    int resolutionWidth, int resolutionHeight)
{
	tiles = Matrix<Tile>(width, height);
	this->width = width;
	this->height = height;
	sf::RectangleShape rectangle;

	// Tile size is scaled depending on resolution to fill the window.
	float sizeX = static_cast<float>(resolutionWidth) / width;
	float sizeY = static_cast<float>(resolutionHeight) / height;
	sf::Vector2f size = { sizeX, sizeY };

	rectangle.setSize(size);
	rectangle.setOutlineThickness(1.0f);

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			Tile tile = Tile(TileType::Wall, rectangle);
			tiles[x][y] = tile;
			tiles[x][y].SetPosition({ x, y });
		}
	}
}

// Creates the actual maze by going to four random
// directions and knocking walls in the path.
void MazeGenerator::Carve(sf::Vector2i currentPosition)
{
	Direction directions[] =
	{ 
	   Direction::Up,
	   Direction::Down,
	   Direction::Left,
	   Direction::Right 
	};

	std::random_shuffle(std::begin(directions), std::end(directions));

	for (int i = 0; i < 4; i++)
	{
		if (!KnockWall(directions[i], currentPosition)) {
			continue;
		}
	}
}

// Tries to knock down a wall from a position to a given direction.
// Returns false if index is out of bounds. Continues carving
// recursively until the whole maze has been formed.
bool MazeGenerator::KnockWall(Direction direction, sf::Vector2i currentPosition)
{
	int& x = currentPosition.x;
	int& y = currentPosition.y;

	switch (direction)
	{
		case Direction::Up:
			if (y <= 2)
				return false;
			if (tiles[x][y - 2].GetType() != TileType::Path)
			{
				tiles[x][y - 1].SetType(TileType::Path);
				tiles[x][y - 2].SetType(TileType::Path);
				Carve({ currentPosition.x, currentPosition.y - 2 });
				return true;
			}
			break;
		case Direction::Down:
			if (y + 2 >= tiles.GetHeight() - 1)
				return false;
			if (tiles[x][y + 2].GetType() != TileType::Path)
			{
				tiles[x][y + 1].SetType(TileType::Path);
				tiles[x][y + 2].SetType(TileType::Path);
				Carve({ currentPosition.x, currentPosition.y + 2 });
				return true;
			}
			break;
		case Direction::Left:
			if (x <= 2)
				return false;
			if (tiles[x - 2][y].GetType() != TileType::Path)
			{
				tiles[x - 1][y].SetType(TileType::Path);
				tiles[x - 2][y].SetType(TileType::Path);
				Carve({ currentPosition.x - 2, currentPosition.y });
				return true;
			}
			break;
		case Direction::Right:
			if (x + 2 >= tiles.GetWidth() - 1)
				return false;
			if (tiles[x + 2][y].GetType() != TileType::Path)
			{
				tiles[x + 1][y].SetType(TileType::Path);
				tiles[x + 2][y].SetType(TileType::Path);
				Carve({ currentPosition.x + 2, currentPosition.y });
				return true;
			}
			break;
	}

	return false;
}

// Creates walls of the Maze by traversing the Matrix and adding
// vertices between centers of adjacent walls.
std::vector<sf::Vertex> 
MazeGenerator::CreateWalls()
{
	std::vector<sf::Vertex> vertices;

	sf::Color wallColor = sf::Color::Green;

	// A line is determined between current wall and adjacent wall for the whole grid.
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			tiles[x][y].SetColor(sf::Color::White);

			if (tiles[x][y].GetType() == TileType::Path || tiles[x][y].GetType() == TileType::Exit)
				continue;

			// Each direction is checked for any adjacent wall tiles, which determines
			// if vertices must be added to the container.

			bool wallRight = (x < width - 1 && tiles[x + 1][y].GetType() == TileType::Wall);
			bool wallLeft  = (x > 0 && tiles[x - 1][y].GetType() == TileType::Wall);
			bool wallDown  = (y < height - 1 && tiles[x][y + 1].GetType() == TileType::Wall);
			bool wallUp    = (y > 0 && tiles[x][y - 1].GetType() == TileType::Wall);

			if (wallRight)
			{
				vertices.push_back({ tiles[x][y].GetCenter(), wallColor });
				vertices.push_back({ tiles[x + 1][y].GetCenter(), wallColor });
			}

			if (wallLeft)
			{
				vertices.push_back({ tiles[x][y].GetCenter(), wallColor });
				vertices.push_back({ tiles[x - 1][y].GetCenter(), wallColor });
			}

			if (wallDown)
			{
				vertices.push_back({ tiles[x][y].GetCenter(), wallColor });
				vertices.push_back({ tiles[x][y + 1].GetCenter(), wallColor });
			}

			if (wallUp)
			{
				vertices.push_back({ tiles[x][y].GetCenter(), wallColor });
				vertices.push_back({ tiles[x][y - 1].GetCenter(), wallColor });
			}
		}
	}

	return vertices;
}

// Returns a random starting position, which must be odd
// to generate a proper maze.
sf::Vector2i MazeGenerator::RandomStartPosition()
{
	using namespace std::chrono;
	auto time = system_clock::now().time_since_epoch();
	milliseconds ms = duration_cast<milliseconds>(time);
	srand(ms.count());

	int x = rand() % width;
	int y = rand() % height;

	while (x % 2 == 0) {
		x = rand() % width;
	}

	while (y % 2 == 0) {
		y = rand() % height;
	}

	return { x, y };
}

// Creates an exit for the maze on one of the border tiles (excluding corners).
// startPosition: The position the player starts from.
void MazeGenerator::CreateRandomExit(sf::Vector2i startPosition)
{
	std::vector<sf::Vector2i> exitPositions;

	// Top and bottom border positions
	for (int x = 1; x < width - 1; x++)
	{
		exitPositions.push_back({ x, 0 });
		exitPositions.push_back({ x, height - 1 });
	}

	// Left and right border positions
	for (int y = 1; y < height - 1; y++)
	{
		exitPositions.push_back({ 0, y });
		exitPositions.push_back({ width - 1, y });
	}

	std::random_shuffle(std::begin(exitPositions), std::end(exitPositions));

	int x = exitPositions[0].x;
	int y = exitPositions[0].y;

	tiles[x][y].SetType(TileType::Exit);

	// There can be a wall tile next to the exit, therefore it must be cleared.
	if (x == 0) {
		tiles[x + 1][y].SetType(TileType::Path);
	}
	else if (x == width - 1) {
		tiles[x - 1][y].SetType(TileType::Path);
	}
	else if (y == 0) {
		tiles[x][y + 1].SetType(TileType::Path);
	}
	else if (y == height - 1) {
		tiles[x][y - 1].SetType(TileType::Path);
	}
}
