// Game_Objects.h //////////////////////////////////////////////////////////////
// All of the core classes that the ignition engine requires to function, //////
// including Newtonian and Keplerian parent classes for all celestial bodies ///
// and vessels /////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include <string>
#include <iostream>
#include <math.h>
#include <vector>
#include "Game_physics.hpp"

#ifndef KeplersObjects
#define KeplersObjects

class Force
{	public:
	// all really just a nice placeholder for two vectors, since a force acting
	// on a body has a point where it acts on the body and a direction
	Force(VectorVictor::Vector2 attack_point, VectorVictor::Vector2 force);
	// simple as it gets, although maybe an alternate for direct variables might
	// be nice here
	VectorVictor::Vector2 Attack_point;
	VectorVictor::Vector2 Force_vector;
	// our vectors...
	double Get_force_torque();
	// basically find the cross product of the two vectors, which should always
	// only have a z component if the vectors are all 2d. This simple version
	// just assumes that the origin we want the waaaiit. Oh shit
	
	// okay, now that should be cleaned up. The basic idea here is that the
	// vanilla Get_force_torque(); assumes our origin to be at (0,0), when we
	// may want to translate it around (center of mass)
	double Get_force_torque(VectorVictor::Vector2 reference_point);
	// and the second version of the function mentioned above, where our origin
	// is shifted by reference_point
	VectorVictor::Vector2 Get_force_vector();
	// just as simple as it sounds
	VectorVictor::Vector2 Get_force_vector(double angle);
	// returns the vector rotated by the given angle clockwise around the origin
	// angle is inputted in degrees. No affect to the direction of the stored
	// Force vector in this object
	// I dont recall where this is used, should look that up
	~Force();
};

// Celestial Bodies ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class CKeplerian_Object
{	public:
	// not sure if keplerian is the right category, but this type is for any
	// body that "moves on rails" since dynmic update of a planet would be
	// agony.
	virtual void Frame(double dt, long double simtime);
	long double Theta, Omega;
	// rotation and angular rate, both stored in degrees, but can be accessed as
	// radians where required
	long double Get_omega();
	// should add this for radians
	long double Get_theta_in_degrees();		
	long double Get_theta_in_radians();
	// self explanatory I think
	
	long double Simulation_time;
	// ahh, this I do not really like, it sort of syncs between the master
	// ignition engine objects simtime, since this object needs to know the
	// current time with precision to know its position (or will soon anyways)
	
	long double Radius;
	// in meters. Sort of a global mean, specific radii will be determined later
	// by  Get_radius(doubl longitude);
	long double Get_radius(double longitude);
	// Simple as it gets, just return the terrain height at given longitude
	// from the planets prime meridian (all the little greenwiches xD)
	// This just returns the constant above for the moment, but the world will
	// not always be a perfect sphere!	
	
	long double Atmosphere_height;
	// distance from the surface to the top of the atmosphere (the point we stop
	// rendering the atmosphere mask at) in meters
	sf::Color Surface_atmosphere_colour;
	// the colour of the atmosphere mask at sea level on the planet
	sf::Color Top_atmosphere_colour;
	// The final colour of the atmosphere mask at an altitude of Atmosphere
	// height above the ground
	
	// of course, the alpha on this  will be 0 at the max height
	
	long double Mass;
	// in kilograms
	long double Get_mass();
	// return for above	
	virtual VectorVictor::Vector2 Get_position(long double sim_time);
	// position at a given time. This is wacky in its current form, better if it
	// were updated every frame and just stored until needed. This was overthink
	// methinks
	void Gravitate(long double satellite_mass, long double satellite_rotation, VectorVictor::Vector2 satellite_position, std::vector<Force> &parent_force_list);
	// new function that tacks its gravity force onto a newtonian object when
	// requested by that function. May not last long given changes in how the 
	// state updaters work, but we will see
	
	VectorVictor::Vector2 Gravity_acceleration(VectorVictor::Vector2 satellite_position, long double simtime);
	
	std::string Object_name;
	// Welcome to <Object_name>!
	std::string Get_object_name();
	// trivial
	sf::Texture * Object_texture;
	// odd, the texture of the planet is global...
	// I think this was related to multiple sprites being generated for
	// different zoom factors, but I dont see a specific reason why they couldnt
	// be stored here instead
	virtual bool In_view(SFML_Window * window, int zoom_factor, long double simtime);	
	// how we check if the planet should be drawn
	virtual void Draw_flag(SFML_Window * iwindow, int zoom_factor);
	// and how we do it if it should
	virtual sf::Color Get_atmosphere_mask(VectorVictor::Vector2 window_origin, long double sim_time);
	// get the colour of the atmosphere that we will render over the starfield
	// as a sf::RectShape in the main window loop
	CKeplerian_Object * Get_keplerian_pointer();
};

class Planetary_surface: public sf::Drawable, public sf::Transformable
{	public:
	bool load(const std::string& tileset, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height);
	private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
};

class TPlanet: public CKeplerian_Object
{	public:
	// basic type derived from Keplerian object that handles official planets
	// and mostly dwarf planets too. Basically anything that orbits the sun and
	// is to big to realistically handle as a newtonian object
	TPlanet(long double initial_theta, long double omega, long double radius, long double atmosphere_height, long double mass, std::string planet_texture_path, sf::Color top_atm_color, sf::Color surf_atm_color);
	void Frame(double dt, long double simtime);
	VectorVictor::Vector2 Get_position(long double sim_time);
	// this... this should work a bit differently, sim_time should be implicitly
	// now, position evaluated and stored each frame
	std::vector<sf::Sprite*> Planet_sprites;
	// the whole sequence of sprites for each successive scaled map view
	// need to remember what the order was for scales
	// after a certain point of zooming out, we just assumed that the image
	// looks the same no matter how much farther we go
	bool In_view(SFML_Window * window, int zoom_factor, long double simtime);
	void Draw_flag(SFML_Window * iwindow, int zoom_factor);	
	// Similar deal as before. Should double check that the inview check uses
	// the correct radius for that map scale
	sf::Color Get_atmosphere_mask(VectorVictor::Vector2 window_origin, long double sim_time);
	
	~TPlanet();
};

#endif

