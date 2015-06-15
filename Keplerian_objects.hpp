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

class Planetary_surface: public sf::Drawable, public sf::Transformable
{	public:
	Planetary_surface();
	Planetary_surface(const std::string& tileset, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height);
	Planetary_surface(const std::string& tileset, sf::Vector2u tileSize, std::vector<int> tiles, unsigned int width, unsigned int height);
	Planetary_surface(sf::Texture &tileset, sf::Vector2u tileSize, std::vector<int> tiles, unsigned int width, unsigned int height);	
	// in practice, each planet will have a list of surface points, which will
	// use a seed to generate the list of values in tiles based on the seed
	// (ie fractally compressed terrain)
	// also the terrain point contains some sort of reference to the type of
	// terrain (grass, desert, water, whatever) 
	
	// not sure what Im gonna do about water though
	// maybe a list of water points that behave like a normal surface tile
	// cause oceans gotta be somewhat rounded if they cover several degrees of
	// longitude and cant be really flat lines across huge distances 
	
	// hmm wait, cant define ocean bottoms that way, maybe just define seas
	// by their beds, and the planetary sea level is used to find where the
	// sea level tiles go from there, that way we can also have underwater
	// dynamics
	
	// but Im going overboard here

	bool load(const std::string& tileset, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height);
	bool load(const std::string& tileset, sf::Vector2u tileSize, std::vector<int> tiles, unsigned int width, unsigned int height);	
	
	bool load(sf::Texture &tileset, sf::Vector2u tileSize, std::vector<int> tiles, unsigned int width, unsigned int height);	
	private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
    public:
    ~Planetary_surface();
};

class Terrain_point
{	Terrain_point();
	Terrain_point(int terrain_type, long double radius, int point_id);
	
	bool Init_object(int terrain_type, long double radius, int point_id);
	// setter
	protected:
	// sigh, I can never remember what these all do...
	int Terrain_type;
	// terrain types determine which texture set is used for the terrain block
	
	// if it goes over the number of types available, we just go to the last one
	// defined, no problem
	long double Radius;
	// the radius of the planet at this given point
	int Point_id;
	// unique id of the point in the list of all of the planets terrain points,
	// used to find the in universe location of the point
	// int Seed;
	// this comin later
	
	public:
	bool Match_point(int point_id);

		
		
	~Terrain_point();
};













typedef id celestial_id;
// an id type for all celestial bodies

class CKeplerian_Object
{	public:
	CKeplerian_Object(long double theta, long double omega, long double mean_radius, long double atm_height, long double mass, std::string texture_path);
	// constructor, fairly generic enough for celestial objects
	static celestial_id Celestial_index;
	// can be seen as either the number of celestial/keplerian objects in the
	// sim, or the id # that will be assigned to the next object of this type
	// created
	
	// used to assign a unique number as an id to each object of this type
	celestial_id Get_new_index();
	// returns a new index to assign to an object and increments the counter
	// by one
	celestial_id Celestial_id;
	// the current objects id #
	celestial_id Get_celestial_id();
	// the global list of all objects of type CKeplerian... and all objects that
	// inherit from this type
	virtual void Frame(double dt, long double simtime);
	// virtual stub for the update function for objects of this type
	long double Theta, Omega;
	// rotation and angular rate, both stored in degrees, but can be accessed as
	// radians where required
	long double Get_omega();
	// by default in degrees
	long double Get_theta_in_degrees();		
	long double Get_theta_in_radians();
	
	long double Simulation_time;
	// ahh, this I do not really like, it sort of syncs between the master
	// ignition engine objects simtime, since this object needs to know the
	// current time with precision to know its position (or will soon anyways)
	
	long double Radius;
	// in meters. Sort of a global mean, specific radii will be determined later
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
	
	//the alphas of both of these are forced to correct values in the
	// constructor, so we dont get any monkey business
	long double Mass;
	// Total mass of the celestial object in kilograms
	long double Get_mass();
	
	virtual VectorVictor::Vector2 Get_position(long double sim_time);
	virtual VectorVictor::Vector2 Get_position();	
	// returns the position of the object at the current time. sim_time
	// required so that we can use the orbit equations in 2d to get an
	// exact position
	VectorVictor::Vector2 Gravity_acceleration(VectorVictor::Vector2 satellite_position, long double simtime);
	// function returning the acceleration due to gravity caused by this body,
	// for the parameters passed
	std::string Object_name;
	// Welcome to <Object_name>!
	std::string Get_object_name();
	
	sf::Texture Object_texture;
	std::vector<sf::Sprite> Object_sprites;
	// the whole sequence of sprites for each successive scaled map view
	// need to remember what the order was for scales
	// after a certain point of zooming out, we just assumed that the image
	// looks the same no matter how much farther we go
	sf::Sprite Object_sprite;
	// the image of the object drawn in the camera view at huge scales, meant
	// to gradually replace the map view in practice
	
	virtual bool In_view(SFML_Window * window, int zoom_factor, long double simtime);	
	virtual bool In_view(SFML_Window * window, long double cam_scale, long double simtime);		
	// how we check if the planet should be drawn
	virtual void Draw_flag(SFML_Window * iwindow, int zoom_factor);
	virtual void Draw_flag(SFML_Window * iwindow, long double cam_scale, long double sim_time);	
	// and how we do it if it should
	virtual sf::Color Get_atmosphere_mask(VectorVictor::Vector2 window_origin, long double sim_time);
	// get the colour of the atmosphere that we will render over the starfield
	// as a sf::RectShape in the main window loop
	
	// this is virtual so that more complex behaviours can eventually be
	// implemented, like gas giant atmospheres with many levels of atmosphere
	virtual int Get_terrain_points();
	// returns the number of points in the terrain total, evenly spaced
	// over the whole planet/moon
	
	// a quick note, based on a maximum value of int of around 2.1 billion,
	// this gives Earth a max density of about 2 cm per point, which
	// should be way better than needed	
	std::vector<Terrain_point> Surface;
	// the list of points that make up the surface, containing info about radii
	// at longitudes & so on
	virtual void Draw_surface(SFML_Window * iwindow);
	// virtual function that draws the surface of the object when in camera view
	CKeplerian_Object * Get_keplerian_pointer();
};












typedef id planet_id;

class TPlanet: public CKeplerian_Object
{	public:
	// basic type derived from Keplerian object that handles official planets
	// and mostly dwarf planets too. Basically anything that orbits the sun and
	// is to big to realistically handle as a newtonian object
	static planet_id Planet_index;
	// can be seen as either the number of planets in the
	// sim, or the id # that will be assigned to the next object of this type
	// created
	
	// used to assign a unique number as an id to each object of this type
	planet_id Get_new_index();
	// returns a new index to assign to an object and increments the counter
	// by one
	planet_id Planet_id;
	// the current objects id #
	planet_id Get_planet_id();
	
	TPlanet(long double initial_theta, long double omega, long double radius, long double atmosphere_height, long double mass, std::string planet_texture_path, sf::Color top_atm_color, sf::Color surf_atm_color);
	void Frame(double dt, long double simtime);
	VectorVictor::Vector2 Get_position(long double sim_time);
	VectorVictor::Vector2 Get_position();	
	// this... this should work a bit differently, sim_time should be implicitly
	// now, position evaluated and stored each frame
	
	bool In_view(SFML_Window * window, int zoom_factor, long double simtime);
	bool In_view(SFML_Window * window, long double cam_scale, long double simtime);	
	void Draw_flag(SFML_Window * iwindow, int zoom_factor);	
	void Draw_flag(SFML_Window * iwindow, long double cam_scale, long double sim_time);	
	// Similar deal as before. Should double check that the inview check uses
	// the correct radius for that map scale
	sf::Color Get_atmosphere_mask(VectorVictor::Vector2 window_origin, long double sim_time);
	
	int Get_terrain_points();
	
	void Draw_surface(SFML_Window * iwindow);
	
	TPlanet * Get_planet_pointer();
	~TPlanet();
};

#endif

