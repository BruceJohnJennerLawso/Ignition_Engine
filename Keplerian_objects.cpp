#include <string>
#include <iostream>
#include <math.h>
#include "Keplerian_objects.hpp"

// Force handling class ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

Force::Force(VectorVictor::Vector2 attack_point, VectorVictor::Vector2 force)
{	Attack_point = attack_point;
	Force_vector = force;
	// simple assignment using standard operators
}

double Force::Get_force_torque(VectorVictor::Vector2 reference_point)
{	double torque = 0;
	VectorVictor::Vector2 new_attack_point = Attack_point;
	new_attack_point -= reference_point;
	// new_attack_point needs to be the relative offset of the attack point from
	// the reference point, probably the new center of mass
	torque += VectorVictor::Get_cross_product(new_attack_point, Force_vector);
	// obtain our torque component in the z axis using the VV function & return
	return torque;
}

// ^ this isnt used yet, but will come into play soon enough

double Force::Get_force_torque()
{	double torque = 0;
	torque += VectorVictor::Get_cross_product(Attack_point, Force_vector);
	// same deal as before, just we assume the reference point is still (0,0)
	return torque;
}

VectorVictor::Vector2 Force::Get_force_vector()
{	return Force_vector;
	// simple return
}

VectorVictor::Vector2 Force::Get_force_vector(double angle)
{	return Force_vector.Get_rotated_vector(angle);
	// same as before, but rotated clockwise about the origin by 'angle' degrees
	// dont recall exactly why this is, here, but coordinate system transforms
	// are probably involved
}

Force::~Force()
{
}


// Celestial Bodies ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void CKeplerian_Object::Frame(double dt, long double simtime)
{	std::cout << "Bad call to CKeplerian_Object::Frame(double dt)" << std::endl;
	// dont wanna call this, would be bad
}

long double CKeplerian_Object::Get_theta_in_degrees()
{	return Theta;
}

long double CKeplerian_Object::Get_theta_in_radians()
{	long double rad_theta = Theta;
	rad_theta *= 6.283185308;		// 4/3 pau actually ;)
	rad_theta /= 360;
	// just converts over to radians, then returns
	return rad_theta;	
}

long double CKeplerian_Object::Get_omega()
{	return Omega;
}

long double CKeplerian_Object::Get_radius(double longitude)
{	return Radius;
	// constant for now, but that will change
}

long double CKeplerian_Object::Get_mass()
{	return Mass;							
}

VectorVictor::Vector2 CKeplerian_Object::Get_position(long double sim_time)
{	//std::cout << "Bad call to CKeplerian_Object::Get_position(double sim_time)" << std::endl;
	VectorVictor::Vector2 origin(0.00000000000000000000,0.00000000000000000000);
	// not sure why all that was necessary, must have been bug-hunting
	return origin;
}	// this should be temporary, planets and moons really need to update
	// their own positions


void CKeplerian_Object::Gravitate(long double satellite_mass, long double satellite_rotation,VectorVictor::Vector2 satellite_position, std::vector<Force> &parent_force_list)
{	long double G = 6.673e-11; 
	long double M = this->Get_mass();	
	VectorVictor::Vector2 origin(0.0000000000000000,0.000000000000000);
	// get the mass of the gravitating body in question
	VectorVictor::Vector2 Vf(0,0);	 
	//this really needs to be fixed to the relative keplerian position
	// was a hack to get things to work, but needs to call the Keplerians
	// position function itself
	
	// * VERY IMPORTANT MUST FIX * //
	Vf -=  satellite_position;
	// Get the relative offset vector between the two objects
	
	long double radial_distance_squared = Vf.Get_vector_magnitude_squared();
	// get the r squared as you would expect
	
	Vf.Normalize();	
	// reduce our direction vector so we have only direction & magnitude 1
		
	long double Fmag = ((G*M)*satellite_mass);
	Fmag /= radial_distance_squared;
	// calculate the value of Fg
	Vf *= Fmag;
	// and multiply our vector by it
		
	long double rotation = (-satellite_rotation);
	// the force is originally in the global coordinates, but it needs to be
	// rotated into the reference frame of the vessels coordinate system
	// this is because the vessel reorients them all when it has its frame
	// call, and to it, a spade is a spade for any Force in the Force vector 
	Force New_force(origin, Vf.Get_rotated_vector(rotation));
	parent_force_list.insert(parent_force_list.end(), New_force); 
	// we construct the Force object, pin it onto the vessel in question
	// and we are done!
}

VectorVictor::Vector2 CKeplerian_Object::Gravity_acceleration(VectorVictor::Vector2 satellite_position, long double simtime)
{	long double G = 6.673e-11; 
	// universal gravitational constant. Because I said so, thats why!!!
	long double M = this->Get_mass();	
	// The mass of the large body doing the pulling here
	
	// sigh...
	
	//VectorVictor::Vector2 origin(0.00,0.00);
	
	
	VectorVictor::Vector2 gravity(0,0);	 
	gravity = this->Get_position(simtime);
	gravity -= satellite_position;
	
	long double radius_squared = gravity.Get_vector_magnitude_squared();
	// get the square of the distance in meters between the two objects
	
	gravity.Normalize();
	// normalize the vector, since we just need the direction of gravity with
	// a magnitude of 1 here
	gravity *= G;
	gravity *= M;
	gravity *= (1/radius_squared);
	// I really need to write that /= operator for VV2
	return gravity;
	// ignore everything after this line, theyre like the siren song of
	// commented code
	
	//this really needs to be fixed to the relative keplerian position
	// was a hack to get things to work, but needs to call the Keplerians
	// position function itself
	
	// * VERY IMPORTANT MUST FIX * //
	//Vf -=  satellite_position;
	// Get the relative offset vector between the two objects
	
	//long double radial_distance_squared = Vf.Get_vector_magnitude_squared();
	// get the r squared as you would expect
	
	//Vf.Normalize();	
	// reduce our direction vector so we have only direction & magnitude 1
		
	//long double Fmag = ((G*M)*satellite_mass);
	//Fmag /= radial_distance_squared;
	// calculate the value of Fg
	//Vf *= Fmag;
	// and multiply our vector by it
		
	//long double rotation = (-satellite_rotation);
	// the force is originally in the global coordinates, but it needs to be
	// rotated into the reference frame of the vessels coordinate system
	// this is because the vessel reorients them all when it has its frame
	// call, and to it, a spade is a spade for any Force in the Force vector 
	//Force New_force(origin, Vf.Get_rotated_vector(rotation));
	//parent_force_list.insert(parent_force_list.end(), New_force); 
	// we construct the Force object, pin it onto the vessel in question
	// and we are done!
}

std::string CKeplerian_Object::Get_object_name()
{	return Object_name;
}	

bool CKeplerian_Object::In_view(SFML_Window * window, int zoom_factor, long double simtime)
{	std::cout << "Bad call to CKeplerian_Object::In_view(SFML_Window * window, int zoom_factor, VectorVictor::Rectangle * view_frame)" << std::endl;
	return false;
}

void CKeplerian_Object::Draw_flag(SFML_Window * iwindow, int zoom_factor)
{	std::cout << "Bad call to CKeplerian_Object::Draw_flag(SFML_Window * iwindow, int zoom_factor, VectorVictor::Rectangle * view_frame)" << std::endl;
}


CKeplerian_Object * CKeplerian_Object::Get_keplerian_pointer()
{	return this;
}

// cant call either of these functions, since info is required that is not
// available at this level. Buuut, if all the Keplerians get a position vector
// variable, it could work!

// changing this should be a priority




// TPlanet /////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////





TPlanet::TPlanet(long double initial_theta, long double omega, long double radius, long double atmosphere_height, long double mass, std::string planet_texture_path)
{	Theta = initial_theta;
	Omega = omega;
	Radius = radius;
	Mass = mass;
	Object_texture = new sf::Texture();
	if(!Object_texture->loadFromFile(planet_texture_path))
	{	std::cout << "Planet " << Get_object_name() << " unable to load texture at " << planet_texture_path << std::endl;
		// Houston, we have a problem.
	}
	else
	{	// the first part was just simple value copying, 
		// but this part gets hairy
		
		// we want to create sprites to represent the body in the map view at
		// multiple scales, specifically powers of 10, so we run a loop
		
		long double pix_length = Get_radius(0) + atmosphere_height;		
		// this was a decent fix for issues that popped up with the terrible
		// png circle the first time around
		
		// our pix length is going to be the diameter of the object in meters
		// ie, the dimensions of the image shown at the greatest zoom factor
		// This is a slight problem since large objects need massively scaled up
		// textures from their original dimensions
		
		// basically the idea here is that we run a loop that creates
		// consecutively smaller images by a factor of 10 to show at zoom levels
		// farther and farther out.
		pix_length /= 5;
		// in other words, multiply our radius by 2 to get diameter, then divide
		// by 10 to get the scale of the first zoom level (zoomed out by 10x)
		float sprite_x, sprite_y;
		// dimensions in pixels of the sprite at the given zoom level 
		for(unsigned int cy = 1; cy != Max_map_scale; ++cy)
		{	sf::Sprite * planet_sprite;
			planet_sprite = new sf::Sprite();
			planet_sprite->setTexture(*Object_texture);
			// we create the sprite at the given zoom level and assign it
			// the default texture
			
			// this really needs to be done without pointers, just pass the
			// reference of a normal object to the vector
			sprite_x = Object_texture->getSize().x;	sprite_x *= planet_sprite->getScale().x;
			sprite_y = Object_texture->getSize().y;  sprite_y *= planet_sprite->getScale().y;
			// for each axis, we find the size of the base texture in pixels,
			// and set the sprite dimensions to it
			
			// the second call involving getScale() seems to be redundant
			// since the scale is always (1,1) they dont seem to do anything
			// needs to be tested more just to be sure though
			std::cout << "sprite dimensions: " << sprite_x << " , " << sprite_y << std::endl;
			
			planet_sprite->setOrigin((sprite_x/2), (sprite_y/2));
			// set the origin of the sprite to the center instead of the top
			// left corner, much eaasier to deal with
			if(Object_texture->getSize().y != pix_length)
			{	planet_sprite->setScale((pix_length/(Object_texture->getSize().x)),(pix_length/(Object_texture->getSize().y)));			
				// unless we get really lucky, the size of the sprite needs to
				// be adjusted to match its size in the game universe.
				// This means multiplying the scale of the object so that
				// the new size of the sprite in pixels is equivalent to the
				// pix length
			}
			planet_sprite->setRotation(Theta);
			Planet_sprites.insert(Planet_sprites.end(), planet_sprite);
			// rotate the sprite to the appropriate angle supplied in the
			// constructor arguments and insert it into the vector of planet
			// sprites at the end. This means that the sprites go from larger
			// to smaller as we go along
			pix_length /= 10;
			// at the end of the loop, scale back the size by 10x, since each
			// consecutive image should be 10x smaller than the previous one
		}
	}
}	// once more, my code works and I dont know why ^^
// I need to ask Vaughan or someone about this
// odd that the relative location of the sprite origin stays the same relative
// to the sprites internal scale when scaled up...


void TPlanet::Frame(double dt, long double simtime)
{	// normal update call for the planet
	Theta += (Omega*dt);
	// update our rotation based on the planets normal spin
	for(std::vector<sf::Sprite*>::iterator it = Planet_sprites.begin(); it != Planet_sprites.end(); ++it)
	{	(*it)->setRotation(Theta);
	}
	// updates the sprites based on current orientation
	// might be better done by the draw call instead, more efficient
	Simulation_time = simtime;
	// and sync the internal clock to the in-universe time
}

VectorVictor::Vector2 TPlanet::Get_position(long double sim_time)
{	VectorVictor::Vector2 origin(0,0);
	// for the moment this is sufficient for testing. Eventually this should
	// just return the planets vector position member, after it is incremented
	// in the frame call
	return origin;
}


bool TPlanet::In_view(SFML_Window * window, int zoom_factor, long double simtime)
{	
	long double radius = this->Get_radius(0);
	return window->Intersection(this->Get_position(simtime), radius);
	// dont know if this will work okay with the call to Get position, this was
	// behaving badly before
	
	
	
	
	
//	if((Get_position(Simulation_time).Get_x() >= (window->origin.x-(Get_radius(0))))&&(Get_position(Simulation_time).Get_x() <= ((window->origin.x + (window->Aperture_width+(Get_radius(0)))))))
//	{	if((Get_position(Simulation_time).Get_y() <= (window->origin.y+(Get_radius(0))))&&(Get_position(Simulation_time).Get_y() >= (window->origin.y - ((window->Aperture_height+(Get_radius(0)))))))
//		{	return true;
//		}
//		else
//		{	return false;
//		}
//	}	
//	else
//	{	return false;
//	}
	// simple algorithm checks if the coordinates of the planet are within the
	// box represented by our camera. Completely useless right now, since
	// the planet isnt even drawn outside of map view, but surface elements
	// will come into play later on
}

void TPlanet::Draw_flag(SFML_Window * iwindow, int zoom_factor)
{	// sounds odd since we dont exactly draw a flag here, but it works anyways
	// could change this so it sounds better I guess... still early on
	//sf::Vector2f offset(Get_position(Simulation_time).Get_x(), Get_position(Simulation_time).Get_y());	
	// we create a sf vector here with the same magnitude as the in-universe
	// position of the planet in question
	//sf::Vector2f camera_origin(iwindow->origin.x, iwindow->origin.y);
	// and we create a second sf vector representing the in-universe offset
	// of the camera (specifically its upper left corner)
	//offset -= camera_origin;
	// we find the relative offset from the camera origin to the planets center
	//offset.y = -offset.y;
	// flip the coordinate system so up is positive y instead of vice-versa
	// (its a peculiarity with how SFML works)
	//offset.y *= pow(0.1, (zoom_factor));
	//offset.x *= pow(0.1, (zoom_factor));
	// and scale back the offsets by 10^zoom_factor
	// since we need to take in game meters and scale them back to pixels
	// on the screen
	
	// okay jitterbug free version:
	
	//VectorVictor::Vector2 offset(Get_position(Simulation_time).Get_x(),  Get_position(Simulation_time).Get_y());
	//VectorVictor::Vector2 camera_origin(iwindow->origin.Get_x(), iwindow->origin.Get_y());
	//offset -= camera_origin;
	//offset.y *= -1;
	//offset.y *= pow(0.1, (zoom_factor));
	//offset.x *= pow(0.1, (zoom_factor));
	
	sf::Vector2f camera_offset = Get_window_coordinates(Get_position(Simulation_time), iwindow, zoom_factor);
	
	
	
	// looks like 1/10, 1/100, 1/1000, ... etc.
	
	for(std::vector<sf::Sprite*>::iterator it = Planet_sprites.begin(); it != Planet_sprites.end(); ++it)
	{	(*it)->setPosition(camera_offset);
	}
	for(std::vector<sf::Sprite*>::iterator it = Planet_sprites.begin(); it != Planet_sprites.end(); ++it)
	{	(*it)->setRotation(Theta - iwindow->Aperture_rotation);
	}
	// eh, just easier to do it this way. I should probably change it
	// eventually so that it only positions the one that we need
	iwindow->window->draw(*(Planet_sprites.at(zoom_factor-1)));
	// pretty simple. Note the zoom - 1 offset to get the right location
	// inside the vector
}

// it would appear that the jitterbug is due to doing those relative offset
// calculations for the position of the image in the 32 bit sf::Vector2f type,
// I *think* this should be fixed by keeping everything up until the actual set
// position call done by VV2s

TPlanet::~TPlanet()
{	delete Object_texture;
}


