// mfd.cpp /////////////////////////////////////////////////////////////////////
// MFD Multifunctional Displays, so we dont have to fly blind anymore //////////
////////////////////////////////////////////////////////////////////////////////
//#include <std_files>
//#include "Headers.h"
//#include "Headers.hpp"
//#include "Source.cpp"
#include "mfd.hpp"

std::string MFD::Get_MFD_Name()
{	Talkback("Bad call to MFD::Get_MFD_Name()");
	return "FUUUUUUUUUUU";
}

bool MFD::Render_MFD(SFML_Window &window, key_commands &keyCommands, Cursor_commands &cursorCommands, long double &cameraScale, long double dt, int &time_acceleration, long double sim_time, TVessel &current_vessel, std::vector<CNewtonian_Object*> &newtonians, std::vector<CKeplerian_Object*> &keplerians, std::vector<TVessel*> &vessels, VectorVictor::Vector2 &Camera_target, long double &Camera_rotation)
{	Talkback("Bad call to MFD::Render_MFD(SFML_Window &window, key_commands &keyCommands, Cursor_commands &cursorCommands, long double &cameraScale, long double dt, int &time_acceleration, long double sim_time, TVessel &current_vessel, std::vector<CNewtonian_Object*> &newtonians, std::vector<CKeplerian_Object*> &keplerians, std::vector<TVessel*> &vessels, VectorVictor::Vector2 &Camera_target, long double &Camera_rotation)");
}

void MFD::Draw_MFD(SFML_Window &window, sf::Color Placard_color)
{	// okay, here we go
	sf::RectangleShape placard(sf::Vector2f(Width, Height));
	placard.setFillColor(Placard_color);
	// we set the background to the mfd to the passed colour
	placard.setPosition(this->Get_mfd_position(window));
	// and we set its position in the window appropriately
	window.window->draw(placard);
	// and draw it to the window
	
	
	// now we need to do the same with whatever the MFD spat out
	sf::Sprite mfd_sprite(MFD_image);
	// create a sprite that uses the texture we generated for the mfd
	mfd_sprite.setPosition(this->Get_mfd_position(window));
	// set its position in the window, same as with the placard background
	window.window->draw(mfd_sprite);
	// and draw it onscreen
}

void MFD::Set_MFD_orientation(orientation new_orientation)
{	MFD_orientation = new_orientation;
}

void MFD::Set_MFD_offset(sf::Vector2f new_offset)
{	MFD_offset = new_offset;
}

sf::Vector2f MFD::Get_mfd_position(SFML_Window &window)
{	sf::Vector2f position(MFD_offset.x, MFD_offset.y);
	if(MFD_orientation == upper_left)
	{	// the simple case, we do as usual
		position = MFD_offset;
		// we set it equal to our offset, easy mode
		return position;
	}
	else if(MFD_orientation == upper_right)
	{	//a wee bit more complex, this time our y axis is normal, but the x axis
		// gets interesting
		float deltax = (float)window.Width - (MFD_offset.x + Width);
		position.x = deltax;
		position.y = MFD_offset.y;
		return position;
	}
	else if(MFD_orientation == lower_left)
	{	// similar situation, 'cept now the y axis is reversed
		float deltay = (float)window.Height - (MFD_offset.y + Height);
		position.y = deltay;
		position.x = MFD_offset.x;
		return position;
	}
	else if(MFD_orientation == lower_right)
	{	// the last case, stuck to the far bottom side of the window
		float deltax = (float)window.Width - (MFD_offset.x + Width);
		float deltay = (float)window.Height - (MFD_offset.y + Height);
		position.x = deltax;
		position.y = deltay;
		return position;
	}
}

sf::Vector2f MFD::Transform_to_mfd(SFML_Window &window, sf::Vector2f input)
{	input -= this->Get_mfd_position(window);
	return input;
}

// Surface MFD /////////////////////////////////////////////////////////////////
// The first prototype /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

Surface_MFD::Surface_MFD()
{	Height = 400;
	Width = 400;
	// these seem like reasonable defaults
	if(!canvas.create(Height, Width))
	{	// something screwed up, maybe we got no more memory to play with
		Talkback("Unable to Create MFD canvas");
	}	// also need to check to see whether the order of height/width is
		// correct	
		
	// the canvas.create function proved insanely slow to create every single
	// call to Render MFD, so we moved it here (ran at barely 3 fps)
	
	// My best idea would maybe be to make it an optional call in Render MFD
	// to create it if the MFD gets resized
	
	if(!Display_font.loadFromFile("./Data/Fonts/orbitron-light.ttf"))
	{	Talkback("Unable to load font for surface MFD");
	}
}

std::string Surface_MFD::Get_MFD_Name()
{	std::string type_name = surface_mfd;
	return type_name;
}

bool Surface_MFD::Render_MFD(SFML_Window &window, key_commands &keyCommands, Cursor_commands &cursorCommands, long double &cameraScale, long double dt, 	int &time_acceleration, long double sim_time, TVessel &current_vessel, std::vector<CNewtonian_Object*> &newtonians, std::vector<CKeplerian_Object*> &keplerians, std::vector<TVessel*> &vessels, VectorVictor::Vector2 &Camera_target, long double &Camera_rotation)
{
	// we create a special sfml object that allows us to draw to it just like
	// it is a screen, even though its not. Then at the end we can retrieve a
	// texture from it and use that for drawing

	canvas.clear();
	
	std::string altitude; 
	if(current_vessel.NewtonianState.Current_state == Flight)
	{	VectorVictor::Vector2 offset(0,0);		
		// if the vessel is in flight, 
		// set the offset to whatever the position of the parent vessel is
		long double separation = 0;
		std::vector<long double> altitude_list;
		for(std::vector<CKeplerian_Object*>::iterator it = keplerians.begin(); it != keplerians.end(); ++it)
		{	separation = 0;
			// ummm
			offset = current_vessel.NewtonianState.FlightState.Position;
			offset -= (*it)->Get_position();
			// make our offset vector equal to the relative offset between
			// the planet and the vessel
			separation = offset.Get_vector_magnitude();
			separation -= (*it)->Get_radius(0);
			// make separation equivalent to the magnitude of the vector in
			// question and subtract the radius from that so we have altitude 
			// instead of radius
			altitude_list.push_back(separation);
			// now we need to find the smallest value of the separation 
		}
		// having run through the list of keplerian objects we need to find the
		// smallest value in the list
		altitude = std::to_string(Smallest_value(altitude_list));

		// awwwwww YEEHAW

	}
	else
	{	altitude = "0 m";
	}

	//altitude = std::to_string(current_vessel->NewtonianState.Current_state);	
	
	//altitude = current_vessel.Get_vessel_name();	
	
	
	
	sf::Text Altitude;
	Altitude.setFont(Display_font);
	Altitude.setColor(sf::Color(157,226,252, 255));
	
	if(cursorCommands.Right_click == true)
	{	sf::Vector2f offset = Transform_to_mfd(window, cursorCommands.position); 
		std::cout << "mfd click offset: (" << offset.x << "," << offset.y << ")" << std::endl; 
		if((offset.x <= Width)&&(offset.y <= Height))
		{	if((offset.x >= 0)&&(offset.y >= 0))
			{
				Altitude.setColor(sf::Color(252, 223, 43, 255));
				// this works, but not very smoothly, since we only get info
				// about clicks every couple milliseconds or so, which makes it
				// very sticky
				
				// perhaps we just force a re render every time a click is 
				// clicked on an MFD to get around this
			}
		}
	}
	
	Circle vessel_bound(current_vessel.NewtonianState.FlightState.Position, (current_vessel.Length/2));
	
	VectorVictor::Vector2 origin(0,0);
	
	Circle target_bound(VectorVictor::Vector2(0,0), 10);
	for(std::vector<CNewtonian_Object*>::iterator it = newtonians.begin(); it != newtonians.end(); ++it)
	{	if((*it) != current_vessel.Get_Newtonian_pointer())
		{	// make sure we dont find ourselves
			target_bound.Set_values((*it)->NewtonianState.FlightState.Position, (((*it)->Length))/2);
			if(vessel_bound.Intersection(target_bound, origin))
			{	Altitude.setColor(sf::Color(252, 0, 0, 255));
				std::cout << "Intersection with object" << std::endl;
			}
			else
			{	std::cout << origin.Get_vector_magnitude() << std::endl;
			}
		}
	}
	
	
	Altitude.setString(altitude);
	// canvas.draw(stuff);
	canvas.draw(Altitude);
	// on the one hand, I could always just write another function that takes
	// our crapload of arguments like before along with the rendertexture canvas
	// by reference and draws it
	
	// but I dont wanna make things more complex just to be idiotproof, so
	// if you are an idiot, GTFO!!!
	
	// If you are new to making mfds and want to learn, just read the SFML
	// tutorial docs on the sfml website regarding graphics and windows and
	// you should have no problem understanding this
	
	canvas.display();
	// not a hundred percent sure what this does, but I think it is necessary
	
	MFD_image = canvas.getTexture();
	return true;
	// we done here, everything went okay
}

Surface_MFD::~Surface_MFD()
{
}

bool Create_MFD(std::string type_name, MFD * &new_mfd, orientation mfd_orientation, sf::Vector2f mfd_offset)
{	if(type_name == surface_mfd)
	{	// think this will work...
		new_mfd = new Surface_MFD();
		new_mfd->Set_MFD_orientation(mfd_orientation);
		new_mfd->Set_MFD_offset(mfd_offset);
		return true;
	}
	else
	{	return false;
	}
}
