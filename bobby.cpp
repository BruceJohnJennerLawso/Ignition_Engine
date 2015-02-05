// bobby.cpp ///////////////////////////////////////////////////////////////////
// base line tests on Vector Victor2 ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//#include <std_files>
//#include "Headers.h"
//#include "Headers.hpp"
//#include "Source.cpp"
#include <iostream>
#include "VectorVictor2.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>


int main()
{
	sf::RenderWindow window(sf::VideoMode(500, 500), "SFML works!");
	sf::CircleShape shape(2);
	shape.setFillColor(sf::Color::Green);
    
	VectorVictor::Vector2 fuubar(10, -10);
    
	sf::CircleShape vector(2);
	vector.setFillColor(sf::Color::Blue);
	vector.setPosition(sf::Vector2f(fuubar.x, -fuubar.y));
    
	sf::Font font;
	if (!font.loadFromFile("./Data/Fonts/orbitron-light.ttf"))
	{	// I dont care right now
	}
	sf::Text text;
	text.setFont(font);
	text.setPosition(sf::Vector2f(0, 470));
	

	while(window.isOpen())
	{	sf::Event event;
		while (window.pollEvent(event))
		{	if(event.type == sf::Event::Closed)
			{	window.close();
			}
			if(event.type == sf::Event::MouseButtonPressed)
			{	if(event.mouseButton.button == sf::Mouse::Button::Left)
				{	sf::Vector2i input_click = sf::Mouse::getPosition(window);
					fuubar.x = input_click.x;
					fuubar.y = -input_click.y;	
				}
				if(event.mouseButton.button == sf::Mouse::Button::Right)
				{	fuubar.Rotate_vector(30);
				}
			}
		}
	    vector.setPosition(sf::Vector2f(fuubar.x, -fuubar.y));
	    text.setString(SI::Get_formatted_value(fuubar.Get_vector_magnitude() , 3, "units"));
	    
		window.clear();
		window.draw(shape);
		window.draw(vector);
		window.draw(text);
		
		window.display();
    }
	return 0;
}
