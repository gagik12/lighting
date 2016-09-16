// Light.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

#include <SFML/Graphics.hpp>

sf::VertexArray shadow(const sf::Shape& caster, sf::Vector2f emitter, sf::Color ambient)
{
	sf::VertexArray geometry(sf::Quads);

	// for each edge of 'shape'.
	const unsigned size = caster.getPointCount();
	for (unsigned i = size - 1, j = 0; j < size; i = j++)
	{
		const auto& xf = caster.getTransform();
		// find world space end points of current edge.
		auto a = xf.transformPoint(caster.getPoint(i));
		auto b = xf.transformPoint(caster.getPoint(j));

		auto ab = b - a;
		auto ea = a - emitter;
		// if current egde faces away from emitter.
		if ((ab.x * ea.y - ab.y * ea.x) < 0.0f)
		{
			// make quad from edge end points and projected end points.
			geometry.append(sf::Vertex(a, ambient));
			geometry.append(sf::Vertex(a + ea, ambient));
			geometry.append(sf::Vertex(b + (b - emitter), ambient));
			geometry.append(sf::Vertex(b, ambient));
		}
	}

	return geometry;
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Shadows");
	window.setFramerateLimit(30);

	// Light blocker.
	sf::RectangleShape shape(sf::Vector2f(100.0f, 100.0f));
	shape.setFillColor(sf::Color::Blue);
	shape.setPosition(250.0f, 250.0f);
	shape.setRotation(60.0f);

	sf::Vector2f emitter;   // Position of light emitter.

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (event.type == sf::Event::MouseMoved)
			{
				// Emitter position tracks mouse pointer.
				emitter = sf::Vector2f(
					static_cast<float>(event.mouseMove.x),
					static_cast<float>(event.mouseMove.y)
				);
			}
		}

		window.clear(sf::Color::White);
		// Draw shadows cast by 'shape'.
		window.draw(shadow(shape, emitter, sf::Color::Black));
		// Dwaw actual 'shape'.
		window.draw(shape);
		window.display();
	}

	return 0;
}
