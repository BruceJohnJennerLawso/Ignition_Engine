#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Ignition_Engine.h"

Ignition_engine::Ignition_engine(std::string title, unsigned int initial_window_width, unsigned int initial_window_height, std::string program_version, double redraw_displays_interval, std::string Intro_audio_path, std::string Game_audio_path)
{	Main_Window = new SFML_Window(title, initial_window_width, initial_window_height);
	// construct a new SFML window with title & size
	commands = new key_commands();
	// just a necessary constructor, nothing to see here
	deltat = 0;
	// or -1 or something. Just a placeholder for some reason
	map_view = false;
	// wanna start in camera view, but other starts will be possible in the 
	// future
	zoom_exponent = 1;
	// same deal as above
	// think this had to be one to satisfy the initial map view conditions, but
	// not 100 % sure. Another area that could use a quick check
	time_acceleration_exponent = 0;
	// same difference
	simulation_time = 0;
	// obvious I think, although this might need modification in the future
	// for starts at different times given that celestial bodies update their
	// positions based on this
	Program_version = program_version;
	// ah I guess the version number based on what the code looked like
	Redraw_interval = redraw_displays_interval;
	// passed through a double indicating how long the refresh interval
	// should be
	Background_tex = new sf::Texture();
	Background_tex->loadFromFile("./Data/Images/starfield.png");
	Background_sprite = new sf::Sprite(*Background_tex);
	// load up the background stars and slot them into their sprite for drawing
	Displays_active = true;
	// just a default behaviour, but again, still toggleable
	Game_audio = new SFML_gameaudio(Intro_audio_path, Game_audio_path);	
	// that butt ugly class for the audio receiving its intro and main musics
	// needs rewriting, but not a critical area at the present time
	//Current_vessel = Vessel_list.at(0);
	// gotta have something for the current vessel, otherwise inputs go to
	// nowhere and that would be bad. later to be loaded from a scn file
}

int Ignition_engine::Ignition()	
{	// All I can think of with this is the Freedom 7 scene from The Right Stuff
	std::string Window_title = "Ignition Engine";
	// or later something program specific
	Window_title.append(Program_version);
	// attach the program version to it so it becomes Ignition Engine 1.0,
	// or something similar. Again, this should eventually only be the
	// default behaviour when a specific name isnt defined
	sf::Clock clock;							
	// Clock used for framerate
	sf::Clock redraw_clock;				
	// Interval timer used to draw the text output displays at a reasonable rate
	// (if the displays update every frame they waste processing power, as well 
	//   as being impossible to read)
	double redraw_timer = 0;
	// gotta start at 0 in order to work back up to the refresh time, when
	// it gets set back to 0 again
	Main_Window->window->setVerticalSyncEnabled(false);
	// this... probably isnt necessary long term, although it probably will make
	// sense to have Vsync on by default
	
	//Main_Window->window->setFramerateLimit(120);
	// quick testing thing from earlier. required a hack on #! to disable vsync
	// so probably useless. 
	Current_vessel = Vessel_list.at(0);
	
	while (Main_Window->window->isOpen())// love how plain language SFML is <3
	{	sf::Event event;
		// some internal thing with SFML inputs
		redraw_timer += redraw_clock.restart().asSeconds();
		// keep updating the time since we last updated the displays
		if(redraw_timer >= Redraw_interval)
		{	Update_text_displays();
			redraw_timer = 0;
			// if the timer goes off, redraw our displays, then reset the timer
			// back to zero so it can work back up to the refresh time
		}
		Main_Window->window->setSize(sf::Vector2u(Main_Window->Width, Main_Window->Height));			
		// forcibly resets the window size to avoid distortion by the user
		// the future feature of smooth transitions will probably change this to
		// a getSize call so that we can pin point where the center now is
		// relative to where it was before
		// actually, hmm... that might be trickier than I first thought
		while (Main_Window->window->pollEvent(event))//Receive and handle inputs
		{	if(event.type == sf::Event::KeyPressed)
			{	Log_keystroke(event.key.code, commands, true);
				// let the event know that a particular key was pressed down
			}
			
			if(event.type == sf::Event::KeyReleased)
			{	Log_keystroke(event.key.code, commands, false);
				// let the event know that a particular key was released
			}
			
			if (event.type == sf::Event::Closed)
			{	Main_Window->window->close();
				// user clicked the little x button on the titlebar, 
				// we're outa here
			}
		}	
		Game_audio->Update_game_audio();		
		// again, no clue how this works now
		Main_Window->window->clear();
		// Clears the main window so that we can begin redrawing stuff onscreen
		
		deltat = clock.restart().asSeconds();
		// Get the length of the previous frame in seconds, which is next passed to the updates
		if(time_acceleration_exponent != 0)
		{	deltat *= (long double)pow((long double)10.00000000000000, (long double)time_acceleration_exponent);
			// pretty sure the long double casts can be removed now
		}	// checks to see if the base length of the frame needs to be
		// modified by the current time acceleration	

		simulation_time += deltat;	
		// increment simtime by the length of the frame in-universe
		
		Current_vessel->Receive_inputs(commands, deltat);	
		// Send any commands that we sent to our key commands object
		// out to the current vessel/input receiver
		
		Main_Window->window->draw(*Background_sprite);																			
		// draw the background. Wish there was some way to keep this drawn to
		// avoid the painful redraw, but no dice. Maybe an OpenGL implementation
		// could...
		switch(map_view)	// kinda hate switches here, if statements might be a better idea																																	
		{	// switch the flow based on whether or not map view is active
			case false:		// map view is inactive, camera view here
			{	for(std::vector<CKeplerian_Object*>::iterator it = Celestial_list.begin(); it != Celestial_list.end(); ++it)
				{	(*it)->Frame(deltat, simulation_time);
				}  // Iterate through all celestial bodies (planets, stars, moons)
					// in the current instance, and update them
				for(std::vector<TVessel*>::iterator it = Vessel_list.begin(); it != Vessel_list.end(); ++it)
				{	// Iterating through all vessels in the current instance
					(*it)->Frame(deltat, simulation_time, Celestial_list);
					// run the vessels frame update based on frame length
				}
					Main_Window->Set_origin((Current_vessel->Position.x - (Main_Window->Aperture_width/2)),(Current_vessel->Position.y + (Main_Window->Aperture_height/2)));		
					// Relocate the window to center on current vessel
					// I always disliked how Orbiter did this, would rather
					// have some sort of polymorphic "targetable" setup, so
					// we dont even need to have vessels in the sim for it to
					// work. Allowing it to focus on a planet, or point, or
					// whatever would be much better
				for(std::vector<TVessel*>::iterator it = Vessel_list.begin(); it != Vessel_list.end(); ++it)
				{	// multiple loops is important here! doing it all in one
					// caused the relative vessel issue (not the jitter though)
					
					// at least one thing verified here:
					// premature optimization really is the root of all evil ;)
					if((*it)->In_view(Main_Window, 0) == true)	// check if the vessel is in view
					{	(*it)->Draw_vessel(Main_Window);																						
						// and draw it if it is. Saves draw calls if the 
						// vessel isnt currently onscreen
						// this in particular seems to be the only major
						// performance bottleneck, around 3000 vessels in sim
						// grinds to about 5-6 FPS. Maybe a solution would be to
						// have a layered box about twice the extent of the
						// current in-game viewport that checks for drawables
						// on a refresh rate like the displays do. A bit clumsy,
						// but it could bring terrific performance increases
					}	
				}	break;	// Break out of the switch for map view and onwards!																		
			}
			case true:	// map view is active	// I guess the order here might work better as state updates, then camera, then draws?
			{	for(std::vector<CKeplerian_Object*>::iterator it = Celestial_list.begin(); it != Celestial_list.end(); ++it)
				{	(*it)->Frame(deltat, simulation_time);
					// Iterate through all celestial bodies (planets, stars, moons)
					// in the current instance, and update them
					Main_Window->Set_origin((Current_vessel->Position.x - (Main_Window->Aperture_width/2)),(Current_vessel->Position.y + (Main_Window->Aperture_height/2)));
					// same deal as camera view, just based on the size of the
					// map view instead
					if((*it)->In_view(Main_Window, zoom_exponent) == true)
					{	(*it)->Draw_flag(Main_Window, zoom_exponent);	
					}	// same problem as before, needs to be optimized where
						// vessels are concerned
				}
				for(std::vector<TVessel*>::iterator it = Vessel_list.begin(); it != Vessel_list.end(); ++it)				// Iterate through all vessels in the current instance
				{	(*it)->Frame(deltat, simulation_time, Celestial_list);																													// Update vessel with frame dt
					Main_Window->Set_origin((Current_vessel->Position.x - (Main_Window->Aperture_width/2)),(Current_vessel->Position.y + (Main_Window->Aperture_height/2)));		// Relocate the window to center on current vessels flag sprite
					if((*it)->In_view(Main_Window, zoom_exponent) == true)														// Check if the flag is within the current camera coordinates
					{	(*it)->Draw_flag(Main_Window, zoom_exponent);																// Draw the flag onscreen
					}	
				}	break;	// Break out of the switch
			}	
		}	
		if(Displays_active == true)	// check if displays are in fact on
		{	Current_vessel->Draw_controls(Main_Window, map_view);
			Redraw_text_displays(map_view, Main_Window);
			// drawing the generic displays, along with the vessel specific ones
		}
		Main_Window->window->display();	// good SFML stuff
    }
    return 0;	// I just love happy endings, dont you?
}

void Ignition_engine::Next_vessel()
{	unsigned int cy = 0;
	for(std::vector<TVessel*>::iterator it = Vessel_list.begin(); it != Vessel_list.end(); ++it)
	{	if(*it == Current_vessel)
		{	if(it == (Vessel_list.end() - 1))
			{	Current_vessel = Vessel_list.at(0);
			}
			else if (it < (Vessel_list.end() -1))
			{	Current_vessel = Vessel_list.at(cy + 1);
			}
			break;	
		}
		cy++;
	}
}

void Ignition_engine::Previous_vessel()
{	unsigned int cy = 0;
	for(std::vector<TVessel*>::iterator it = Vessel_list.begin(); it != Vessel_list.end(); ++it)
	{	if(*it == Current_vessel)
		{	if(it == Vessel_list.begin())
			{	Current_vessel = Vessel_list.at(Vessel_list.size() - 1);
			}
			else if (it > Vessel_list.begin())
			{	Current_vessel = Vessel_list.at(cy - 1);
			}
			break;
		}
		cy++;
	}
}

void Ignition_engine::Map_view()
{	map_view = true;
	// flip the bool so we know which mode we are in
	Set_aperture_scale();
	// and resize the Main windows aperture to match the current map view
	
}

void Ignition_engine::Increase_map_scale()
{	if(zoom_exponent < (Max_map_scale -1))
	{	// minus one cause if we're at the max map scale, we cant go up any higher
		zoom_exponent++;
		Set_aperture_scale();
	}
}

	
void Ignition_engine::Decrease_map_scale()
{	if(zoom_exponent >= 2)
	{	zoom_exponent--;
		Set_aperture_scale();
	}
}

void Ignition_engine::Set_aperture_scale()
{	if(map_view == true)
	{	Main_Window->Set_aperture_dimensions(((Main_Window->Width)*(pow(10, zoom_exponent))),((Main_Window->Height)*(pow(10, zoom_exponent))));
	}
	else if(map_view == false)
	{	Main_Window->Set_aperture_dimensions(((long double)Main_Window->Width/10),((long double)Main_Window->Height/10));
	}
}
	
void Ignition_engine::Camera_view()
{	map_view = false;
	// flip the bool so we know which mode we are in
	Set_aperture_scale();
	// and resize the Main windows aperture to match the current map view
}

Ignition_engine::~Ignition_engine()
{	delete commands;
	delete Main_Window;
	delete Background_sprite;
	delete Background_tex;
	delete Game_audio;
}	// so, yeah...

