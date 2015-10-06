#ifndef PLAYER_H
#define PLAYER_H

#include "SFML\Graphics.hpp"
#include <memory>
#include "Maze.h"

class Player : public sf::Drawable
{
	public:
		Player(sf::Vector2f size, const std::shared_ptr<Maze>& maze);
		void Update();
		bool IsAtExit();
		void SetPosition(int x, int y);
		void SetMaze(const std::shared_ptr<Maze>& maze);
		void GotoStart();
	private:
		int X {0};
		int Y {0};
		sf::RectangleShape rectangle;
		std::shared_ptr<Maze> maze;

		void Move(int x, int y);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		void UpdateWorldPosition();
};

#endif