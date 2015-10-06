#include <SFML\Graphics.hpp>
#include "MazeGenerator.h"
#include "Application.h"
#include "Player.h"
#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
	int width = 640; int height = 640; 
	int size = 21;
	Application application(width, height);

	MazeGenerator mazeGenerator;
	std::shared_ptr<Maze> maze = mazeGenerator.Create(size, size, width, height);

	sf::Vector2f playerSize = { static_cast<float>(width) / size, 
		                        static_cast<float>(height) / size };

	Player player(playerSize, maze);
	player.GotoStart();

	maze->Solve();

	while (application.Run())
	{
		player.Update();
		if (player.IsAtExit())
		{
			maze = mazeGenerator.Create(size, size, width, height);
			player.SetMaze(maze);
			player.GotoStart();
			maze->Solve();
		}

		application.Draw(*maze);
		application.Draw(player);
		application.Display();
	}

	return 0;
}