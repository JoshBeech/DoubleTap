#include <SFML/Graphics.hpp>
#include <TGUI\TGUI.hpp>

int main()
{
	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	tgui::Gui theGui{ window };
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			theGui.handleEvent(event);

			if (event.type == sf::Event::Closed)
				window.close();
		}

		theGui.draw();
		window.clear();
		window.draw(shape);
		window.display();
	}

	return 0;
}