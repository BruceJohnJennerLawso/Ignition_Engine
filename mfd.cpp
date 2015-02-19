// mfd.cpp /////////////////////////////////////////////////////////////////////
// MFD Multifunctional Displays, so we dont have to fly blind anymore //////////
////////////////////////////////////////////////////////////////////////////////
//#include <std_files>
//#include "Headers.h"
//#include "Headers.hpp"
//#include "Source.cpp"
#include "mfd.hpp"


bool MFD::Render_MFD(SFML_Window &window, key_commands &keyCommands, Cursor_commands &cursorCommands, long double &cameraScale, long double dt, 	int &time_acceleration, long double sim_time, TVessel &current_vessel, std::vector<CNewtonian_Object*> &newtonians, std::vector<CKeplerian_Object*> &keplerians, std::vector<TVessel*> &vessels, VectorVictor::Vector2 &Camera_target, long double &Camera_rotation)
{	Talkback("Bad call to MFD::Render_MFD(SFML_Window &window, key_commands &keyCommands, Cursor_commands &cursorCommands, long double &cameraScale, long double dt, 	int &time_acceleration, long double sim_time, TVessel &current_vessel, std::vector<CNewtonian_Object*> &newtonians, std::vector<CKeplerian_Object*> &keplerians, std::vector<TVessel*> &vessels, VectorVictor::Vector2 &Camera_target, long double &Camera_rotation)");
}

void MFD::Draw_MFD(SFML_Window &window, sf::Color Placard_color)
{	// okay, here we go
	sf::RectangleShape placard(Height, Width);
	placard.setColor(Placard_color);
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

sf::Vector2f MFD::Get_mfd_position(SFML_Window &window)
{	sf::Vector2f position();
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


bool Surface_MFD::Render_MFD(SFML_Window &window, key_commands &keyCommands, Cursor_commands &cursorCommands, long double &cameraScale, long double dt, 	int &time_acceleration, long double sim_time, TVessel &current_vessel, std::vector<CNewtonian_Object*> &newtonians, std::vector<CKeplerian_Object*> &keplerians, std::vector<TVessel*> &vessels, VectorVictor::Vector2 &Camera_target, long double &Camera_rotation)
{
	sf::RenderTexture canvas;
	// we create a special sfml object that allows us to draw to it just like
	// it is a screen, even though its not. Then at the end we can retrieve a
	// texture from it and use that for drawing
	if(!canvas.create(Height, Width))
	{	// something screwed up, maybe we got no more memory to play with
		Talkback("Unable to Create MFD canvas");
		return false;
	}	// also need to check to see whether the order of height/width is
		// correct
	canvas.clear();
	
	// we set up any old drawable and go
	
	// canvas.draw(stuff);
	
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
