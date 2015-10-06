#include "Application.h"

Application::Application(int width, int height) : window(sf::VideoMode(width, height), "Maze Generator",
												  sf::Style::Default, sf::ContextSettings(0, 0, 4, 3, 0)),
												  width(width),
												  height(height)
{
	window.setFramerateLimit(24);
}

bool Application::Run()
{
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				window.close();
				return false;
			}
		}

		return true;
	}
}

void Application::Draw(const sf::Drawable& drawable)
{
	window.draw(drawable);
}

void Application::Display()
{
	window.display();
	window.clear(sf::Color(40, 40, 40));
}