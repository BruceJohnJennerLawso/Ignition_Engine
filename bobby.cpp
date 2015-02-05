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
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(2);
    shape.setFillColor(sf::Color::Green);
    
    VectorVictor::Vector2 fuubar(10, -10);
    
    sf::CircleShape vector(2);
    vector.setFillColor(sf::Color::Blue);
    vector.setPosition(sf::Vector2f(fuubar.x, -fuubar.y));
    
    

    while (window.isOpen())
    {	sf::Event event;
		while (window.pollEvent(event))
		{	if (event.type == sf::Event::Closed)
            {	window.close();
			}
		}
		window.clear();
		window.draw(shape);
		window.draw(vector);
		window.display();
    }
    return 0;
}
