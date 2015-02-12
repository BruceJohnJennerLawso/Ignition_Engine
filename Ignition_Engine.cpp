#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Ignition_Engine.hpp"



Ignition_engine::Ignition_engine(std::string title, unsigned int initial_window_width, unsigned int initial_window_height, std::string program_version, double redraw_displays_interval, std::string standard_display_font, std::string starfield)
{	Main_Window = new SFML_Window(title, initial_window_width, initial_window_height);
	// construct a new SFML window with title & size
	commands = new key_commands();
	cursor_commands = new Cursor_commands();
	// just a necessary constructors, nothing to see here
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
	Background_tex->loadFromFile(starfield);
	Background_sprite = new sf::Sprite(*Background_tex);
	// load up the background stars and slot them into their sprite for drawing
	
	// center the sprite onscreen
	
	double width_value = (double)initial_window_width;
	double height_value = (double)initial_window_height;	
	double field_scale = Larger_of((width_value/height_value), (height_value/width_value));
	Background_sprite->setScale((field_scale),(field_scale));		
	
	double sprite_x = Background_tex->getSize().x;
	double sprite_y = Background_tex->getSize().y;

	Background_sprite->setOrigin((sprite_x/2), (sprite_y/2));
	Background_sprite->setPosition(sf::Vector2f(sprite_x/2, sprite_y/2));
	
	// lets try without this and see what happens
	
	// the background sprite is being naughty here, gotta figure out some sort
	// of workaround to make this work better
	Displays_active = true;
	// just a default behaviour, but again, still toggleable
	double Larger_of(double value1, double value2);
	camera_scale = 1.00;
	k_camera = 9.5;
	
	if(standard_font.loadFromFile(standard_display_font))
	{	sf::Color init_colour(50, 255, 255);
		standard_display_colour = init_colour;
		// think this works
		if(this->Init_standard_displays())
		{	// I dunno, but we are done here
		}
		else
		{	Talkback("Unable to init standard displays");
		}
	}
	else
	{	Talkback("Unable to load standard font at: ");
		Talkback(standard_display_font);
	}
	
}


bool Ignition_engine::Init_standard_displays()
{	
	
	fps_meter.Init_object(standard_font, sf::Vector2f(20, 40), "0", standard_display_colour, 15, false);
	map_scale_meter.Init_object(standard_font, sf::Vector2f(20, 60), "0", standard_display_colour, 15, false);
	time_accel_meter.Init_object(standard_font, sf::Vector2f(20, 80), "0", standard_display_colour, 15, false);
	sim_time_meter.Init_object(standard_font, sf::Vector2f(20, 100), "0", standard_display_colour, 15, false);
	
	camera_target_name.Init_object(standard_font, sf::Vector2f(875, 40), "0", standard_display_colour, 15, false);
	
	
	return true;
}

void Ignition_engine::Update_standard_displays()
{	if(Displays_active == true)
	{	unsigned int fps = (1/deltat);
		std::string framerate = SI::Get_formatted_value("FPS:", (long int)fps, 3, "");
		fps_meter.Set_element(framerate);
		// we set the fps meter
		std::string zoomfactor = SI::Get_formatted_value("Map Scale:", ((long int)pow((long double)10, (int)zoom_exponent)), "x");
		map_scale_meter.Set_element(zoomfactor);
		// and then set the map scale meter based on '10^zoom_exponent x'"
		std::string timewarp = SI::Get_formatted_value( "Time Accel:", ((long int)pow((long double)10, (int)time_acceleration_exponent)), "x");
		time_accel_meter.Set_element(timewarp);
		// and do the same thing for time acceleration
		std::string simtime = SI::Get_formatted_value("Simulation Time:", ((long int)simulation_time) , "s");
		sim_time_meter.Set_element(simtime);
		// set the simulation time, with sim time cast as an int cause we like
		// it that way
		camera_target_name.Set_element(Current_vessel->Get_vessel_name());
		// and do things directly with setting the current vessel name
		
		// *** This WILL be changed later ***
		// specifically vessel name will not always work like this
	}
}

void Ignition_engine::Draw_standard_displays()
{	this->fps_meter.Draw_element(Main_Window);
	this->map_scale_meter.Draw_element(Main_Window);
	this->time_accel_meter.Draw_element(Main_Window);
	this->sim_time_meter.Draw_element(Main_Window);
	this->camera_target_name.Draw_element(Main_Window);
}


int Ignition_engine::Ignition()	
{	// All I can think of with this is the Freedom 7 scene from The Right Stuff
	std::cout << "Value of cam rotation: " << Camera_rotation << std::endl;
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
	this->Position_window(Current_vessel);	
	// we want to set it to something here, because it cant just be left
	// without a value, or we will crash needless to say
	
	// so we set it to the first vessel in the main vessel list 
	
	while (Main_Window->window->isOpen())
	{	sf::Event event;
		// some internal thing with SFML inputs
		redraw_timer += redraw_clock.restart().asSeconds();
		// keep updating the time since we last updated the displays
		if(redraw_timer >= Redraw_interval)
		{	this->Update_standard_displays();
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
		{	// we check the main window for any events received
			if(event.type == sf::Event::KeyPressed)
			{	Log_keystroke(event.key.code, commands, true);
				// let the event know that a particular key was pressed down
				// on the keyboard
				this->Handle_inputs();
			}
			
			if(event.type == sf::Event::KeyReleased)
			{	Log_keystroke(event.key.code, commands, false);
				// let the event know that a particular key on the keyboard
				// was released in this frame
				
			}
			
			if(event.type == sf::Event::Closed)
			{	Main_Window->window->close();
				// user clicked the little x button on the titlebar, 
				// we're outa here
				
				// the destructor handles any closing up stuff like deleting
				// pointers within the object, so all we need to do here is make
				// sure to close the window
			}
			if(event.type == sf::Event::MouseWheelMoved)
			{	if(map_view == false)
				{	// make sure that we are in the normal camera view, just
					// because we dont want to screw with the state of one view
					// or another if we arent in it at the moment
					
					// not a big deal, but it works better this way, smoother
					// interfacing with the user IMO
					Change_camera_scale(- (double)event.mouseWheel.delta);
					// if the mouse wheel was moved, used the number of ticks thatdouble Larger_of(double value1, double value2);
					// it moved by to change the scale of the current camera view
				}
			}
			if(event.type == sf::Event::MouseMoved)
			{	cursor_commands->Set_cursor_state(((event.mouseMove.x)), ((event.mouseMove.y)), false, false, false);
				// now that we get the coords from mouseMove, this works just fine
			}
			if(event.type == sf::Event::MouseButtonPressed)
			{	// if one of the mouse buttons gets pressed, we update the
				// cursor commands object that contains info about the state
				// of the cursor to reflect its new state
				
				// the cursor commands object stores this, so if the button
				// is pressed, it is considered pressed (or true) until
				// MouseButtonReleased sets it back to false
				if(event.mouseButton.button == sf::Mouse::Button::Left)
				{	cursor_commands->Set_cursor_state(event.mouseButton.x, event.mouseButton.y, false, true, false);
				}
				if(event.mouseButton.button == sf::Mouse::Button::Right)
				{	cursor_commands->Set_cursor_state(event.mouseButton.x, event.mouseButton.y, true, false, false);
				}
				if(event.mouseButton.button == sf::Mouse::Button::Middle)
				{	cursor_commands->Set_cursor_state(event.mouseButton.x, event.mouseButton.y, false, false, true);
				}
			}
			if(event.type == sf::Event::MouseButtonReleased)
			{	// if the button on the mouse was released, we need to set the
				// state of the button stored by cursor commands back to false
				// so that any objects receiving the cursor commands dont think
				// the button in question is still pressed
				if(event.mouseButton.button == sf::Mouse::Button::Left)
				{	cursor_commands->Set_cursor_state(event.mouseButton.x, event.mouseButton.y, false, false, false);
				}
				if(event.mouseButton.button == sf::Mouse::Button::Right)
				{	cursor_commands->Set_cursor_state(event.mouseButton.x, event.mouseButton.y, false, false, false);
				}
				if(event.mouseButton.button == sf::Mouse::Button::Middle)
				{	cursor_commands->Set_cursor_state(event.mouseButton.x, event.mouseButton.y, false, false, false);
				}
			}
			
			// this whole section above could probably be separated into a
			// function... hmm
		}	
		Main_Window->window->clear();
		// Clears the main window so that we can begin redrawing stuff onscreen
		
		deltat = clock.restart().asSeconds();
		// Get the length of the previous frame in seconds, which is next
		// used by the sim to step forward in time
		
		// also resets the sfml clock to 0 so the next frame is measured
		// properly 
		if(time_acceleration_exponent != 0)
		{	deltat *= (long double)pow((long double)10.00000000000000, (long double)time_acceleration_exponent);
			// if the time_acceleration exponent is not zero, we need to
			// multiply deltat by a power of 10 so that the frame is 10^n x
			// longer in the simulations time than it is in real life. This is
			// basically because no sane person has a week straight to sit in
			// front of their computer doing a moon mission
			// (or god forbid a mars flight)
			// so we need to make the sim run faster
			
			// pretty sure the long double casts can be removed now
			// I think they were just a form of desperate debugging of some type
			// to remove a problem that has since evaporated
		}		

		simulation_time += deltat;	
		// increment simtime by the length of the frame in-universe
		
		Current_vessel->Receive_inputs(commands, deltat);	
		// Send any commands that we sent to our key commands object
		// out to the current vessel/input receiver
		
		Background_sprite->setRotation(-Main_Window->Aperture_rotation);
		// okay, lets try this again
		
		// this was screwy because I didnt think it through fully enough
		// the problem is actually much more complex than I thought
		
		Main_Window->window->draw(*Background_sprite);																			
		// draw the background. Wish there was some way to keep this drawn to
		// avoid the painful redraw, but no dice. Maybe an OpenGL implementation
		// could...
		if(map_view == false)
		{	// map view is inactive, camera view here
			for(std::vector<CKeplerian_Object*>::iterator it = Celestial_list.begin(); it != Celestial_list.end(); ++it)
			{	(*it)->Frame(deltat, simulation_time);
			}  // Iterate through all celestial bodies (planets, stars, moons)
				// in the current instance, and update them
			for(std::vector<TVessel*>::iterator it = Vessel_list.begin(); it != Vessel_list.end(); ++it)
			{	// Iterating through all vessels in the current instance
				(*it)->Frame(deltat, simulation_time, Celestial_list);
				// run the vessels frame update based on frame length,
				// and send along the list of celestial bodies in the sim,
				// so that it can get gravitational forces from them
					
				// should also pass along the Newtonian and Vessel type
				// lists for convenience too methinks
			}
			
			//Main_Window->Set_origin((Current_vessel->NewtonianState.FlightState.Position.x - (Main_Window->Aperture_width/2)),(Current_vessel->NewtonianState.FlightState.Position.y + (Main_Window->Aperture_height/2)));		
			
			this->Position_window(Current_vessel);
			// easy mode ladies & germs
			
			
			// Relocate the window to center on current vessel
			// I always disliked how Orbiter did this, would rather
			// have some sort of polymorphic "targetable" setup, so
			// we dont even need to have vessels in the sim for it to
			// work. Allowing it to focus on a planet, or point, or
			// whatever would be much better
			
			for(std::vector<CKeplerian_Object*>::iterator it = Celestial_list.begin(); it != Celestial_list.end(); ++it)
			{	sf::RectangleShape atmosphere(sf::Vector2f(Main_Window->Width, Main_Window->Height));
				atmosphere.setFillColor((*it)->Get_atmosphere_mask(Camera_target, simulation_time));
				Main_Window->window->draw(atmosphere);
			}
			
			
			for(std::vector<TVessel*>::iterator it = Vessel_list.begin(); it != Vessel_list.end(); ++it)
			{	// multiple loops is important here! doing it all in one
				// caused the relative vessel issue (not the jitter though)
				
				// at least one thing verified here:
				// premature optimization really is the root of all evil ;)
				if((*it)->In_view(Main_Window, 0) == true)	// check if the vessel is in view
				{	(*it)->Draw_vessel(Main_Window, camera_scale);																						
					// and draw it if it is. Saves draw calls if the 
					// vessel isnt currently onscreen
					// this in particular seems to be the only major
					// performance bottleneck, around 3000 vessels in sim
					// grinds to about 5-6 FPS. Maybe a solution would be to
					// have a layered box about twice the extent of the
					// current in-game viewport that checks for drawables
					// on a refresh rate like the displays do. A bit clumsy,
					// but it could bring terrific performance increases
					
					(*it)->Receive_cursor_inputs(cursor_commands, deltat);
					// since cursor commands can apply to anything visible
					// in the screen we want to send them out to every
					// vessel in the sim, so that the vessels can transform
					// the window local vector into a simulation coordinate
					// and decide what to do with the info they receive at
					// that point
				}	
			}
		}	
		else if(map_view == true)	// map view is active	// I guess the order here might work better as state updates, then camera, then draws?
		{	for(std::vector<CKeplerian_Object*>::iterator it = Celestial_list.begin(); it != Celestial_list.end(); ++it)
			{	(*it)->Frame(deltat, simulation_time);
				// Iterate through all celestial bodies (planets, stars, moons)
				// in the current instance, and update them

				// same deal as camera view, just based on the size of the
				// map view instead
			}
			for(std::vector<TVessel*>::iterator it = Vessel_list.begin(); it != Vessel_list.end(); ++it)				// Iterate through all vessels in the current instance
			{	(*it)->Frame(deltat, simulation_time, Celestial_list);	
			}	
			//Main_Window->Set_origin((Current_vessel->NewtonianState.FlightState.Position.x - (Main_Window->Aperture_width/2)),(Current_vessel->NewtonianState.FlightState.Position.y + (Main_Window->Aperture_height/2)));
			this->Position_window(Current_vessel);
			for(std::vector<CKeplerian_Object*>::iterator it = Celestial_list.begin(); it != Celestial_list.end(); ++it)
			{	// same deal as camera view, just based on the size of the
				// map view instead
				if((*it)->In_view(Main_Window, zoom_exponent, simulation_time) == true)
				{	(*it)->Draw_flag(Main_Window, zoom_exponent);	
				}	// same problem as before, needs to be optimized where
						// vessels are concerned
			}
			for(std::vector<TVessel*>::iterator it = Vessel_list.begin(); it != Vessel_list.end(); ++it)				// Iterate through all vessels in the current instance
			{	if((*it)->In_view(Main_Window, zoom_exponent) == true)														// Check if the flag is within the current camera coordinates
				{	(*it)->Draw_flag(Main_Window, zoom_exponent);	
					// Draw the flag onscreen
					(*it)->Receive_cursor_inputs(cursor_commands, deltat);
				}	
			}	
		}	
		if(Displays_active == true)	// check if displays are in fact on
		{	Current_vessel->Draw_controls(Main_Window, map_view);
			this->Draw_standard_displays();
			// drawing the generic displays, along with the vessel specific ones
		}
		Main_Window->window->display();	// good SFML stuff
    }
    return 0;	// I just love happy endings, dont you?
}

void Ignition_engine::Handle_inputs()
{	if(this->commands->comma == true)
	{	this->Previous_vessel();
	}
	else if(this->commands->period == true)
	{	this->Next_vessel();
	}	
	
	if(this->commands->space == true)
	{	this->Current_vessel->Print_data();
	}
	
	if(this->commands->dash == true)
	{	if(map_view == true)
		{	this->Increase_map_scale();
		}
		else
		{	this->Increase_camera_scale();
			// the change of the scale of the camera view once, that part is 
			//implemented
		}
	}
	else if(this->commands->equal == true)
	{	if(map_view == true)
		{	this->Decrease_map_scale();
		}
		else
		{	this->Decrease_camera_scale();
			// the change of the scale of the camera view once, that part is 
			//implemented
		}
	}
	
	if(this->commands->v == true)
	{	if(map_view == false)
		{	this->Map_view();
		}
		else if(map_view == true)
		{	this->Camera_view();
		}
	}
	
	if(this->commands->r == true)
	{	this->Decrease_time_acceleration();
	}
	else if(this->commands->t == true)
	{	this->Increase_time_acceleration();double Larger_of(double value1, double value2);
	}
	
	if(this->commands->i == true)
	{	if(Displays_active == true)
		{	Displays_active = false;
			this->Update_standard_displays();
		}
		else
		{	Displays_active = true;
			this->Update_standard_displays();
		}
	}
		
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
			this->Update_standard_displays();
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
			this->Update_standard_displays();
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
	this->Update_standard_displays();
	
}

void Ignition_engine::Increase_map_scale()
{	if(zoom_exponent < (Max_map_scale -1))
	{	// minus one cause if we're at the max map scale, we cant go up any higher
		zoom_exponent++;
		Set_aperture_scale();
		this->Update_standard_displays();
	}
}

	
void Ignition_engine::Decrease_map_scale()
{	if(zoom_exponent > Min_map_scale)
	{	zoom_exponent--;
		Set_aperture_scale();
		this->Update_standard_displays();
	}
}

void Ignition_engine::Set_aperture_scale()
{	if(map_view == true)
	{	Main_Window->Set_aperture_dimensions(((Main_Window->Width)*(pow((long double)10, (int)zoom_exponent))),((Main_Window->Height)*(pow((long double)10, (int)zoom_exponent))));
	}
	else if(map_view == false)
	{	Main_Window->Set_aperture_dimensions((( (long double)Main_Window->Width * camera_scale)/10),(( (long double)Main_Window->Height * camera_scale)/10));
	}
}
	
void Ignition_engine::Camera_view()
{	map_view = false;
	// flip the bool so we know which mode we are in
	Set_aperture_scale();
	// and resize the Main windows aperture to match the current map view
	this->Update_standard_displays();
}

void Ignition_engine::Increase_camera_scale()
{	if(camera_scale < Max_cam_scale)
	{	camera_scale += (deltat*((long double)k_camera));
		if(camera_scale > Max_cam_scale)
		{	camera_scale = Max_cam_scale;
		}
		else if(camera_scale < Min_cam_scale)
		{	camera_scale = Min_cam_scale;
		}
		Set_aperture_scale();
		this->Update_standard_displays();
	}
}

void Ignition_engine::Change_camera_scale(double wheel_delta)
{	if((camera_scale <= Max_cam_scale)&&(camera_scale >= Min_cam_scale))
	{	camera_scale += (deltat*((long double)k_camera)*((long double)wheel_delta));
	
		if(camera_scale > Max_cam_scale)
		{	camera_scale = Max_cam_scale;
		}
		else if(camera_scale < Min_cam_scale)
		{	camera_scale = Min_cam_scale;
		}
		Set_aperture_scale();
	}
	else
	{	// we do nuthin, cause
	}
}

void Ignition_engine::Decrease_camera_scale()
{	if(camera_scale > Min_cam_scale)
	{	camera_scale -= (deltat*((long double)k_camera));
		if(camera_scale > Max_cam_scale)
		{	camera_scale = Max_cam_scale;
		}
		else if(camera_scale < Min_cam_scale)
		{	camera_scale = Min_cam_scale;
		}
		Set_aperture_scale();
		this->Update_standard_displays();
	}
}

void Ignition_engine::Decrease_time_acceleration()
{	if(time_acceleration_exponent >= 1)
	{	time_acceleration_exponent--;
		this->Update_standard_displays();
	}
}	// slow down time acceleration by 10 times

void Ignition_engine::Increase_time_acceleration()
{	if(time_acceleration_exponent < 10)
	{	time_acceleration_exponent++;
		this->Update_standard_displays();
	}
}	// speed up time acceleration by 10 times


void Ignition_engine::Position_window(TVessel * target_vessel)
{	this->Set_aperture_scale();
	// just checking for funny business here...
	// seemed to help a little bit anyways
	
	Camera_rotation = target_vessel->NewtonianState.Rotation.Theta;
	Camera_target.Set_values(-(Main_Window->Aperture_width/2), (Main_Window->Aperture_height/2));
	Camera_target.Rotate_vector(Camera_rotation);
	// okay there is something definitely wrong here if the length of the vector
	// is deciding to be variable
	
	// rotate it around so we get the correct direction	
	Camera_target += target_vessel->NewtonianState.FlightState.Position;	
	// wow, I am just not with it today...
	Main_Window->Set_origin(Camera_target.x, Camera_target.y);
	Main_Window->Set_aperture_rotation(Camera_rotation);
	// set our window to the correct location and we are good to go
}

Ignition_engine::~Ignition_engine()
{	
	delete commands;
	delete cursor_commands;
	delete Main_Window;
	delete Background_sprite;
	delete Background_tex;
}	// so, yeah...

double Larger_of(double value1, double value2)
{	if(value1 > value2)
	{	return value1;
	}
	else if(value1 == value2)
	{	return value1;
		// dunno, just works okay, what difference does it make?
	}
	else
	{	return value2;
	}
}
