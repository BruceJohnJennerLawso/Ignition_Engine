#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

Ignition_engine::Ignition_engine(std::string title, unsigned int initial_window_width, unsigned int initial_window_height, std::string program_version, double redraw_displays_interval, std::string Intro_audio_path, std::string Game_audio_path)
{	Main_Window = new SFML_Window(title, initial_window_width, initial_window_height);
	commands = new key_commands();
	deltat = 0;
	map_view = false;
	zoom_exponent = 1;
	time_acceleration_exponent = 0;
	simulation_time = 0;	
	Program_version = program_version;
	Redraw_interval = redraw_displays_interval;
	
	Background_tex = new sf::Texture();
	Background_tex->loadFromFile("./Data/Images/starfield.png");
	Background_sprite = new sf::Sprite(*Background_tex);
	
	Displays_active = true;
	
	Game_audio = new SFML_gameaudio(Intro_audio_path, Game_audio_path);	
	//Game_audio = new SFML_gameaudio("./Data/Audio/Menu_Fade_In.ogg",  "./Data/Audio/Game_music_Yavin.ogg");	
	
	Current_vessel = Vessel_list.at(0);
}

int Ignition_engine::Ignition()
{	std::string Window_title = "Ignition Engine";
	Window_title.append(Program_version);
	sf::Clock clock;							// Clock used for framerate
	sf::Clock redraw_clock;				// Interval timer used to draw the text output displays at a reasonable rate (if the displays update every frame they waste processing power, as well as being impossible to read)
	double redraw_timer = 0;
	Main_Window->window->setVerticalSyncEnabled(false);
	Main_Window->window->setFramerateLimit(120);
	while (Main_Window->window->isOpen())																									// while loop depending on the window being open
	{	sf::Event event;
		if(spill_frame == true)
		{	spill_frame = false;
			std::cout << "\n" << std::endl;
		}
		redraw_timer += redraw_clock.restart().asSeconds();
		if(redraw_timer >= Redraw_interval)
		{	Update_text_displays();
			redraw_timer = 0;
		}
		Main_Window->window->setSize(sf::Vector2u(Main_Window->Width, Main_Window->Height));			// Resets the window size to avoid distortion
		while (Main_Window->window->pollEvent(event))																					// Receives and handles the inputs sent
		{	if(event.type == sf::Event::KeyPressed)
			{	Log_keystroke(event.key.code, commands, true);
			}
			
			if(event.type == sf::Event::KeyReleased)
			{	Log_keystroke(event.key.code, commands, false);
			}
			
			if (event.type == sf::Event::Closed)
			{	Main_Window->window->close();
			}
		}	Game_audio->Update_game_audio();		// this is screwed up now... not sure how best to fix  dont want the current crappy object I wrote in this																									// Frame update the audio
		Main_Window->window->clear();																											// Clears the main window so that we can begin redrawing stuff onscreen
		
		deltat = clock.restart().asSeconds();																										// Get the length of the previous frame in seconds, which is next passed to the updates																						// Send inputs to the current vessel
		if(time_acceleration_exponent != 0)																										// If our time acceleration is nonzero
		{	deltat *= (long double)pow((long double)10.00000000000000, (long double)time_acceleration_exponent);	// long double ALL THE THINGS!!!																			// adjust the deltat by the current time acceleration factor (since it should be that much longer than realtime was)
		}	Spillback("Frame length", deltat);	
		simulation_time += deltat;																													// increment simtime by the time ran
		
		Current_vessel->Receive_inputs(commands, deltat);																			// Send any commands that we received out to the current vessel/input receiver
		
		Main_Window->window->draw(*Background_sprite);																			// draw the background. Wish there was some way to keep this drawn to avoid the painful redraw
		switch(map_view)																																	
		{	// switch the flow based on whether or not map view is active
			case false:																																			// map view is inactive
			{	for(std::vector<CKeplerian_Object*>::iterator it = Celestial_list.begin(); it != Celestial_list.end(); ++it)
				{	(*it)->Frame(deltat, simulation_time);
				}
				for(std::vector<TVessel*>::iterator it = Vessel_list.begin(); it != Vessel_list.end(); ++it)				// Iterate through all vessels in the current instance
				{	(*it)->Frame(deltat, simulation_time);																													// Update vessel with frame dt
					Main_Window->Set_origin((Current_vessel->Position.x - (Main_Window->Width/2)),(Current_vessel->Position.y + (Main_Window->Height/2)));		// Relocate the window to center on current vessel				
					if((*it)->In_view(Main_Window, 0) == true)																				// check if the vessel is in view
					{	(*it)->Draw_vessel(Main_Window);																						// and draw it if it is. Saves draw calls if the vessel isnt currently onscreen
					}	
				}	break;																																			// Break out of the switch for map view																		
			}
			case true:																																			// map view is active
			{	for(std::vector<CKeplerian_Object*>::iterator it = Celestial_list.begin(); it != Celestial_list.end(); ++it)
				{	(*it)->Frame(deltat, simulation_time);
					Main_Window->Set_origin((Current_vessel->Position.x - (Main_Window->Aperture_width/2)),(Current_vessel->Position.y + (Main_Window->Aperture_height/2)));		// Relocate the window to center on current vessels flag sprite
					if((*it)->In_view(Main_Window, zoom_exponent) == true)														// Check if the flag is within the current camera coordinates
					{	(*it)->Draw_flag(Main_Window, zoom_exponent);																// Draw the flag onscreen
					}
				}
				for(std::vector<TVessel*>::iterator it = Vessel_list.begin(); it != Vessel_list.end(); ++it)				// Iterate through all vessels in the current instance
				{	(*it)->Frame(deltat, simulation_time);																													// Update vessel with frame dt
					Main_Window->Set_origin((Current_vessel->Position.x - (Main_Window->Aperture_width/2)),(Current_vessel->Position.y + (Main_Window->Aperture_height/2)));		// Relocate the window to center on current vessels flag sprite
					if((*it)->In_view(Main_Window, zoom_exponent) == true)														// Check if the flag is within the current camera coordinates
					{	(*it)->Draw_flag(Main_Window, zoom_exponent);																// Draw the flag onscreen
					}	
				}	break;																																			// Break out of the switch
			}	
		}	
		if(Displays_active == true)
		{	Current_vessel->Draw_controls(Main_Window, map_view);
		}	Redraw_text_displays(map_view, Main_Window);																				// Update the text displays via function call. Only happens if the displays are active already	
		Main_Window->window->display();																										// Displays the window
    }
    return 0;																																						// I just love happy endings, dont you?
}

Ignition_engine::~Ignition_engine()
{	delete commands;
	delete Main_Window;
	delete Background_sprite;
	delete Background_tex;
	delete Game_audio;
}

