// bobby.cpp ///////////////////////////////////////////////////////////////////
// base line tests on Vector Victor2 ///////////////////////////////////////////
// shes got a bobby pin!!! /////////////////////////////////////////////////////
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
	// create da window
	sf::CircleShape shape(2);
	shape.setFillColor(sf::Color::Green);
	shape.setPosition(sf::Vector2f(250,250));
    // set up a small green circle at the origin, (0,0), the upper left hand
    // corner of the screen in this case
	VectorVictor::Vector2 fuubar1(10, -10);
    // create the VV2 object that we are testing on
	sf::CircleShape vector1(2);
	vector1.setFillColor(sf::Color::Blue);
	vector1.setPosition(sf::Vector2f(fuubar1.x, -fuubar1.y));

	VectorVictor::Vector2 fuubar2(10, -10);
    // create the VV2 object that we are testing on
	sf::CircleShape vector2(2);
	vector2.setFillColor(sf::Color::Yellow);
	vector2.setPosition(sf::Vector2f(fuubar2.x, -fuubar2.y));
	
	
	VectorVictor::Vector2 fuubar3(10, -10);
    // create the VV2 object that we are testing on
	sf::CircleShape vector3(2);
	vector3.setFillColor(sf::Color(128, 0, 128));
	vector3.setPosition(sf::Vector2f(fuubar2.x, -fuubar2.y));	
    
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
					fuubar1.x = -250 +  input_click.x;
					fuubar1.y = 250 -input_click.y;	
				}
				if(event.mouseButton.button == sf::Mouse::Button::Right)
				{	sf::Vector2i input_click = sf::Mouse::getPosition(window);
					fuubar2.x = -250 + input_click.x;
					fuubar2.y = 250 -input_click.y;
				}
			}
		}
		fuubar3 = fuubar1.Projection(fuubar2);
		
	    vector1.setPosition(sf::Vector2f(250 + fuubar1.x, 250-fuubar1.y));
	    vector2.setPosition(sf::Vector2f(250 +fuubar2.x,250 -fuubar2.y));	    
	    vector3.setPosition(sf::Vector2f(250 +fuubar3.x,250 -fuubar3.y));	    	    
	    
	    std::string output_text = SI::Get_formatted_value(fuubar1.Get_vector_magnitude() , 3, "units	");
	    
	    if(fuubar2.Parallel(fuubar1))
	    {    output_text.append("Parallel");
			// well, at least we know that works
		}
	    //VectorVictor::Vector_pair a(fuubar1, (fuubar2 - fuubar1));
	    //VectorVictor::Vector_pair b(fuubar1, (fuubar3 - fuubar1));
	    
		//output_text.append(Orientation_as_text(a.Orientation(b)));	    
		// this is definitely not working right at the moment, perhaps a little
		// bit of tinkering can kick it into gear
		
		// it thinks that anything parallel is nonparallel, and vice versa
		
		// for now, we just leave it
	    text.setString(output_text);
	    
		window.clear();
		window.draw(shape);
		window.draw(vector1);
		window.draw(vector2);		
		window.draw(vector3);				
		window.draw(text);
		
		window.display();
    }
	return 0;
}
