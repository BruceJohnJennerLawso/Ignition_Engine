// Lightspeed //////////////////////////////////////////////////////////////////
// It aint like dusting crops //////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#include "Ignition_Engine.h"

#ifdef LINUX
#include "Ignition_Engine.cpp"
#include "Inertia_moment.cpp"
#include "SFML_Tools.cpp"
#include "Game_Objects.cpp"
#include "VectorVictor2.cpp"
#endif

#define  Starfighter_version " 0.06"

// Engine Pointers /////////////////////////////////////////////////////////

Ignition_engine * Starfighter;

SFML_titlescreen * Title_screen;
sf::Clock * Utility_clock;

sf::Font * displays_font;
std::vector<sf::Text> Data_displays;


sf::Texture * GCW_Flags_tex;

sf::Texture * XWing_tex;
sf::Texture * XWing_status_tex;
sf::Texture * XWing_panel_tex;

sf::Sprite * Rebel_flag_sprite;
sf::Sprite * Imperial_flag_sprite;
TVessel * Rogue9, * Champion7, * Champion8, * Rogue_leader;
TVessel * Eyeball1, * Eyeball2;

CKeplerian_Object * Earth;

vector2 spawn_point;
double init_theta;
bool spawn_flipper;

//template <class T> void Iterate_forward(std::vector<T> &target_vector, std::vector<T>::iterator it, T &target_value)		// References are needed for the second pass, nicer to have for the first
//{	//std::vector<T>::iterator it;
//	unsigned int cy = 0;
//	for(it = target_vector.begin(); it != target_vector.end(); ++it)
//	{	if((*it) == target_value)
//		{	if(it == target_vector.begin())
//			{	target_value = target_vector.at(target_vector.size() - 1);
//			}
//			else if (it > target_vector.begin())
//			{	target_value = target_vector.at(cy - 1);
//			}
//			break;
//		}
//		cy++;
//	}
//}





////////////////////////////////////////////////////////////////////////////

void Intro_splash_screen();
void Update_audio();

void Set_current_vessel(TVessel * target_vessel);



void Update_text_displays()
{	switch(Starfighter->Displays_active)
	{	case true:
		{	unsigned int fps = (1/Starfighter->deltat);
			std::string framerate;	framerate = "FPS: ";	framerate.append(std::to_string(fps));
			(Data_displays.at(0)).setString(framerate);
			(Data_displays.at(1)).setString(Starfighter->Current_vessel->Get_vessel_name());
			std::string zoomfactor; zoomfactor = "Map scale: "; zoomfactor.append(std::to_string((long int)pow(10, Starfighter->zoom_exponent)));
			(Data_displays.at(2)).setString(zoomfactor);
			std::string timewarp; timewarp = "Time Accel: "; timewarp.append(std::to_string((long int)pow(10, Starfighter->time_acceleration_exponent)));	timewarp.append("x");
			(Data_displays.at(3)).setString(timewarp);
			std::string simtime = "Simulation Time: ";
			simtime.append(std::to_string((long int)Starfighter->simulation_time));	simtime.append(" s");
			(Data_displays.at(4)).setString(simtime);
			
		}
		case false:
		{	
		}
	}
}

void Redraw_text_displays(bool in_map_view, SFML_Window * draw_window)
{	switch(Starfighter->Displays_active)
	{	case true:
		{	for(std::vector<sf::Text>::iterator it = Data_displays.begin(); it != Data_displays.end(); ++it)
			draw_window->window->draw(*it);
		}
		case false:
		{
		}
	}
}

// 0	frame rate
// 1	map scale
// 2	time acceleration factor
// 3	mission time
// 4
// 5

int main()
{	Init_assets();								// Create the assets used by the game
	std::string Window_title = "Ignition Engine";
	Window_title.append(Starfighter_version);
	Starfighter = new Ignition_engine(Window_title, 609, 1024, Starfighter_version, 0.2, "./Data/Audio/Menu_Fade_In.ogg",  "./Data/Audio/Game_music_Yavin.ogg");	
	while(Starfighter->Main_Window->window->isOpen())
	{	sf::Event event;
		while (Starfighter->Main_Window->window->pollEvent(event))
		{	if (event.type == sf::Event::Closed)
            {	Starfighter->Main_Window->window->close();
				
			}
			if(event.type == sf::Event::KeyPressed)
			{	Log_keystroke(event.key.code, Starfighter->commands, true);
			}
			if(event.type == sf::Event::KeyReleased)
			{	Log_keystroke(event.key.code, Starfighter->commands, false);
			}
			if(Title_screen->finished == true)
			{	Starfighter->Ignition();	
			}
        }
   		Starfighter->Main_Window->window->setSize(sf::Vector2u(Starfighter->Main_Window->Width, Starfighter->Main_Window->Height));
		Starfighter->Main_Window->window->clear();
		Title_screen->Update_screen();
		Starfighter->Main_Window->window->draw(*(Title_screen->splash_sprite));
		Starfighter->Main_Window->window->draw(*(Title_screen->Title_text));
		Starfighter->Main_Window->window->display();
	}	
	Starfighter->Ignition();																				// And we have a liftoff!!! WOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO 
	Exit_program();																						// Function cleans up our pointers before exit
    return 0;																									// I just love happy endings, dont you?
}

void key_commands::Enter()
{	Title_screen->Toggle_fade();
}

void key_commands::Space()
{	std::cout << "Window aperture w,h   " << Starfighter->Main_Window->Aperture_width << " , " << Starfighter->Main_Window->Aperture_height << std::endl;

	std::cout << "Frame Rate   " << (1/Starfighter->deltat) <<  "   Zoom factor:   "  << Starfighter->zoom_exponent << "   pow(zoom factor)   " << pow(10, Starfighter->zoom_exponent) << "   \n" << std::endl;

	std::cout << Starfighter->Current_vessel->Get_vessel_name() << std::endl;
	std::cout << "Rot   " << Starfighter->Current_vessel->Get_theta_in_degrees() << std::endl;
	std::cout << "Omega   " << Starfighter->Current_vessel->Get_omega() << std::endl;
	std::cout << "PMI " << Starfighter->Current_vessel->Get_PMI() << std::endl;
	std::cout << "x   " << Starfighter->Current_vessel->Position.x << " y   " << Starfighter->Current_vessel->Position.y << std::endl;
	std::cout << "vx   " << Starfighter->Current_vessel->Velocity.x << " vy   " << Starfighter->Current_vessel->Velocity.y << std::endl;
	Starfighter->Current_vessel->Print_data();
	//std::cout << "throttle   " << Current_vessel->Main_engines.Thruster_throttle << std::endl << std::endl;		// need to reroute this
	//std::cout << Vessel_list.size() << " vessels, size " << sizeof(TVessel) << std::endl;
	//for(std::vector<TVessel*>::iterator it = Vessel_list.begin(); it != Vessel_list.end(); ++it)
	//{	std::cout << (*it)->name << "	" << (*it)->In_view(Main_Window, zoom_exponent) << std::endl;
	//}
}

void key_commands::Comma()
{	unsigned int cy = 0;
	for(std::vector<TVessel*>::iterator it = Vessel_list.begin(); it != Vessel_list.end(); ++it)
	{	if(*it == Starfighter->Current_vessel)
		{	if(it == Vessel_list.begin())
			{	Starfighter->Current_vessel = Vessel_list.at(Vessel_list.size() - 1);
			}
			else if (it > Vessel_list.begin())
			{	Starfighter->Current_vessel = Vessel_list.at(cy - 1);
			}
			break;
		}
		cy++;
	}
	//Iterate_forward<TVessel*>(Vessel_list, it, Starfighter->Current_vessel);
	Update_text_displays();
}

void key_commands::Period()
{	unsigned int cy = 0;
	for(std::vector<TVessel*>::iterator it = Vessel_list.begin(); it != Vessel_list.end(); ++it)
	{	if(*it == Starfighter->Current_vessel)
		{	if(it == (Vessel_list.end() - 1))
			{	Starfighter->Current_vessel = Vessel_list.at(0);
			}
			else if (it < (Vessel_list.end() -1))
			{	Starfighter->Current_vessel = Vessel_list.at(cy + 1);
			}
			break;	
		}
		cy++;
	}
	Update_text_displays();
}

void key_commands::Up()
{	//Current_vessel->throttle_up = true;
}

void key_commands::Left()
{	//Current_vessel->rot_left = true;
}

void key_commands::Right()
{	//Current_vessel->rot_right = true;
	//Game_audio->Music_mode(false);
}

void key_commands::Down()
{	//Current_vessel->throttle_down = true;
}

void key_commands::Tilde()
{	
}

void key_commands::Dash()
{	if(Starfighter->zoom_exponent < 6)	// greater than 6 blows up because our width integer goes out of bounds and resets itself 
	{	Starfighter->zoom_exponent++;
		Starfighter->Main_Window->Set_aperture_dimensions(((Starfighter->Main_Window->Aperture_width)*(10)),((Starfighter->Main_Window->Aperture_height)*(10)));
		Update_text_displays();
	}
	else
	{	// Super map? Evidently this will require something a little bit more complex, since the data limits on the unsigned int aperture size get overrun when going up to 10^7
	}	// current limits are roughly 1 million km by 500-600 thousand, plenty for most any planetary/moons system, but not enough for interplanetary travel

}

void key_commands::Equal()
{	if(Starfighter->zoom_exponent >= 2)
	{	Starfighter->zoom_exponent--;
		double h = Starfighter->Main_Window->Aperture_height;
		double w = Starfighter->Main_Window->Aperture_width;
		h /= 10;	w /= 10;
		Starfighter->Main_Window->Set_aperture_dimensions(w, h);
		Update_text_displays();
	}
}

void key_commands::V()
{	if(Starfighter->map_view == false)
	{	Starfighter->map_view = true;
	}
	else if(Starfighter->map_view == true)
	{	Starfighter->map_view = false;
	}
}

void key_commands::N()
{	spawn_point.x += 5;
	switch(spawn_flipper)
	{	case false:
		{	spawn_point.y += 100;
			spawn_flipper = true;
			init_theta += 90;
			while(init_theta >= 360)
			{	init_theta -= 360;
			}	TVessel * new_vessel;	new_vessel = new XWing(spawn_point.x, spawn_point.y, 0, 0, init_theta, 0, 1000, 1600, Rebel_flag_sprite, XWing_tex, "Vessel", XWing_status_tex, displays_font, XWing_panel_tex);
			Vessel_list.insert(Vessel_list.end(), new_vessel);
			//Vessel_list.emplace_back((spawn_point.x, spawn_point.y, 0, 0, init_theta, 0, 70.4, 29000, 8000, 16, "./Data/Images/Ywing_AlanHart.png", Rebel_flag_sprite, "Champion 8"));
			break;
		}
		case true:
		{	spawn_point.y -= 200;
			spawn_flipper = false;
			init_theta += 90;
			while(init_theta >= 360)
			{	init_theta -= 360;
			}	TVessel * new_vessel;	new_vessel = new XWing(spawn_point.x, spawn_point.y, 0, 0, init_theta, 0, 1000, 1600, Rebel_flag_sprite, XWing_tex, "Vessel", XWing_status_tex, displays_font, XWing_panel_tex);
			Vessel_list.insert(Vessel_list.end(), new_vessel);
			break;
		}
	}
}

// (double ix, double iy, double ivx, double ivy, double irot, double iomega, double idomega, double imass, double ithrust, double ilength, std::string texture_path, sf::Sprite * iFlag_sprite, std::string iname)

void key_commands::Q()
{	
}

void key_commands::W()
{	
}

void key_commands::E()
{	
}

void key_commands::A()
{	
}

void key_commands::S()
{	
}

void key_commands::D()
{	
}

void key_commands::F()
{	
	#ifdef SPILLBACK 
	spill_frame = true;
	#endif
}

void key_commands::Z()
{	
}

void key_commands::X()
{	
}	// This will take some time I guess

void key_commands::C()
{	
}

void key_commands::R()
{	if(Starfighter->time_acceleration_exponent >= 1)
	{	Starfighter->time_acceleration_exponent--;
		Update_text_displays();
	}
}

void key_commands::T()
{	if(Starfighter->time_acceleration_exponent < 10)
	{	Starfighter->time_acceleration_exponent++;
		Update_text_displays();
	}
}

void key_commands::L()
{
}

void key_commands::I()
{	if(Starfighter->Displays_active == true)
	{	Starfighter->Displays_active = false;
	}
	else
	{	Starfighter->Displays_active = true;
		Update_text_displays();
	}
}

void key_commands::Numpad_0()
{	std::cout << "Numpad_0() called" << std::endl;
}

void key_commands::Numpad_1()
{	std::cout << "Numpad_1() called" << std::endl;
}

void key_commands::Numpad_2()
{	std::cout << "Numpad_2() called" << std::endl;
}

void key_commands::Numpad_3()
{	std::cout << "Numpad_3() called" << std::endl;
}

void key_commands::Numpad_4()
{	std::cout << "Numpad_4() called" << std::endl;
}

void key_commands::Numpad_5()
{	std::cout << "Numpad_5() called" << std::endl;
}

void key_commands::Numpad_6()
{	std::cout << "Numpad_6() called" << std::endl;
}

void key_commands::Numpad_7()
{	std::cout << "Numpad_7() called" << std::endl;
}

void key_commands::Numpad_8()
{	std::cout << "Numpad_8() called" << std::endl;
}

void key_commands::Numpad_9()
{	std::cout << "Numpad_9() called" << std::endl;
}

void key_commands::Plus()
{	std::cout << "Plus() called" << std::endl;
}

void key_commands::Minus()
{	std::cout << "Minus() called" << std::endl;
}




void Init_assets()
{	Talkback("Initializing assets");
	spawn_point.x = 0; spawn_point.y = 0;	spawn_flipper = false;	init_theta = 0;
	displays_font = new sf::Font();
	displays_font->loadFromFile("./Data/Fonts/orbitron-light.ttf");
	Title_screen = new SFML_titlescreen("./Data/on_a_wing_and_a_prayer_by_unusualsuspex-d4wh7et.png", true, 0.700, 0.900, " ", "./Data/Fonts/Stjldbl1.ttf", 252, 223, 43, 72, sf::Vector2f(280, 50));
	Utility_clock = new sf::Clock();
	GCW_Flags_tex = new sf::Texture();
	GCW_Flags_tex->loadFromFile("./Data/Images/Galactic_civil_war.png");
	Rebel_flag_sprite = new sf::Sprite(*GCW_Flags_tex);
	Rebel_flag_sprite->setTextureRect(sf::IntRect(0,200,200,200));
	Imperial_flag_sprite = new sf::Sprite(*GCW_Flags_tex);
	Imperial_flag_sprite->setTextureRect(sf::IntRect(0,0,200,200));
	XWing_tex = new sf::Texture;
	XWing_tex->loadFromFile("./Data/Images/dg.png");
	XWing_status_tex = new sf::Texture;
	XWing_status_tex->loadFromFile("./Data/Images/dg.png");
	XWing_status_tex->setSmooth(true);
	XWing_panel_tex = new sf::Texture();
	XWing_panel_tex->loadFromFile("./Data/Images/XWing_display_panel.png");
	std::cout << "Loaded XWing_tex from file" << std::endl;
	Rogue9 = new XWing(6678000.00000000000000000, 0.0000000000000, 0.000000000000, 18000.00000000000000, 270.0000000000, 0, 40000, 20600, Rebel_flag_sprite, XWing_tex, "GL-01", XWing_status_tex, displays_font, XWing_panel_tex); 
	Rogue_leader = new XWing(6678000.000000000000000, -12.00000000000000, 0.00000000000000, 18000.00000000000, 180, 0, 40000, 20600, Rebel_flag_sprite, XWing_tex, "GL-02", XWing_status_tex, displays_font, XWing_panel_tex);
	//Eyeball1 = new TVessel(900, -400, 2, 4, 220, 0, 90.5, 9000, 40000, 6, "./Data/Images/TieFighter_AlMeerow.png", Imperial_flag_sprite, "Eyeball 1");
	//Eyeball2 = new TVessel(4000, -700, 0, 0, 15, 0, 90.7, 9000, 40000, 6, "./Data/Images/TieFighter_AlMeerow.png", Imperial_flag_sprite, "Eyeball 2");
	//Champion7 = new TVessel(8000, -700, 0, 0, 190, 0, 70.4, 29000, 8000, 16, "./Data/Images/Ywing_AlanHart.png", Rebel_flag_sprite, "Champion 7");
	//Champion8 = new TVessel(8500, -900, 0, 0, 190, 0, 70.4, 29000, 8000, 16, "./Data/Images/Ywing_AlanHart.png", Rebel_flag_sprite, "Champion 8");
	//Vessel_list.emplace_back((Rogue9));
	Vessel_list.insert(Vessel_list.end(), Rogue9);
	Vessel_list.insert(Vessel_list.end(), Rogue_leader);
	
	Newtonian_list.insert(Newtonian_list.end(), Rogue9->Get_Newtonian_pointer());
	Newtonian_list.insert(Newtonian_list.end(), Rogue_leader->Get_Newtonian_pointer());
	
	Earth = new TPlanet(0.000, 0.0000727, 6378100, 245000, 5.9736e24, "./Data/Images/Planets/earth.png");
	Celestial_list.insert(Celestial_list.end(), Earth);
	//Vessel_list.insert(Vessel_list.end(), Eyeball1);
	//Vessel_list.insert(Vessel_list.end(), Eyeball2);
	//Vessel_list.insert(Vessel_list.end(), Champion7);
	//Vessel_list.insert(Vessel_list.end(), Champion8);
	std::cout << "Finished constructing Vessels" << std::endl;
	//Current_vessel = Rogue_leader;
	//Current_vessel = Rogue9;
	std::cout << "Initializing text displays" << std::endl;
	Init_text_displays();
	std::cout << "Finished initializing text displays" << std::endl;
	std::cout << "Vessel list size " << Vessel_list.size() << " Newtonian list size " << Newtonian_list.size() << std::endl;
}

void Init_text_displays()
{	Data_displays.emplace_back("0" , *displays_font, 30);
	(Data_displays.at(0)).setCharacterSize(15);
	(Data_displays.at(0)).setColor(sf::Color(50, 255, 255));
	(Data_displays.at(0)).setPosition(20, 40);
	(Data_displays.at(0)).setString(std::to_string(0));
	Data_displays.emplace_back("0" , *displays_font, 30);
	(Data_displays.at(1)).setCharacterSize(15);
	(Data_displays.at(1)).setColor(sf::Color(50, 255, 255));
	(Data_displays.at(1)).setPosition(875, 40);
	(Data_displays.at(1)).setString(std::to_string(0));
	Data_displays.emplace_back("0" , *displays_font, 30);
	(Data_displays.at(2)).setCharacterSize(15);
	(Data_displays.at(2)).setColor(sf::Color(50, 255, 255));
	(Data_displays.at(2)).setPosition(20, 60);
	(Data_displays.at(2)).setString(std::to_string(0));
	Data_displays.emplace_back("0" , *displays_font, 30);
	(Data_displays.at(3)).setCharacterSize(15);
	(Data_displays.at(3)).setColor(sf::Color(50, 255, 255));
	(Data_displays.at(3)).setPosition(20, 80);
	(Data_displays.at(3)).setString(std::to_string(0));
	Data_displays.emplace_back("0" , *displays_font, 30);
	(Data_displays.at(4)).setCharacterSize(15);
	(Data_displays.at(4)).setColor(sf::Color(50, 255, 255));
	(Data_displays.at(4)).setPosition(20, 100);
	(Data_displays.at(4)).setString(std::to_string(0));
}

void Exit_program()
{	delete Title_screen;
	for(std::vector<TVessel*>::iterator it = Vessel_list.begin(); it != Vessel_list.end(); ++it)
	{	delete (*it);
	}	delete XWing_tex;
	delete XWing_status_tex;
	delete XWing_panel_tex;
	delete Rebel_flag_sprite;
	delete Imperial_flag_sprite;
	delete GCW_Flags_tex;
	delete Starfighter;
}

