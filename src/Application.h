#ifndef _APPLICATION_H
#define _APPLICATION_H

#include "SFML\Graphics.hpp"
#include <functional>

class Application
{
	public:
		Application(int width, int height);
		bool Run();
		int GetWidth() const;
		int GetHeight() const;
		void Draw(const sf::Drawable& drawable);
		void Display();

	private:
		sf::RenderWindow window;
		int width;
		int height;
};

#endif