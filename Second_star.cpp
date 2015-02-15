// Second star to the right ////////////////////////////////////////////////////
// And straight on till we hit the precision limits of long double /////////////
////////////////////////////////////////////////////////////////////////////////

#include "Ignition_Engine.hpp"
#define  Ignition_version " 0.20"	// see? progress ;)
// this should be renamed to reflect the move away from Starfighter


// Engine Pointers /////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// these pointers really need to be reduced to bare minimum if possible

// EVIL EVIL IS ERRYWHERE

Ignition_engine * Ignition_testing;
// the main ignition engine object


SFML_titlescreen * Title_screen;
// the splash screen before the program really gets goings 



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

// this seemed legit, but I got a weird issue when trying to compile :(



////////////////////////////////////////////////////////////////////////////

void Intro_splash_screen();
void Update_audio();
// havent a clue what these do. Better find out


void Set_current_vessel(TVessel * target_vessel);
// augh mine eyes are bleeding, whyyyyyyy



// 0	frame rate
// 1	map scale
// 2	time acceleration factor
// 3	simulation time
// 4
// 5

// Ahh, this can go, not relevant anymore


int main()
{
	std::string Window_title = "Ignition Engine";
	// gotta give our baby a name
	Window_title.append(Ignition_version);
	// stick the version numbah on the end
	std::cout << "Constructing Ignition Engine" << std::endl;
	Ignition_testing = new Ignition_engine(Window_title, 609, 1024, Ignition_version, 0.2, "./Data/Fonts/orbitron-light.ttf", "./Data/Images/starfield.png");	
	// give birth to our beautiful new engine object. Isnt it cute?
	std::cout << "Right before Init_assets()" << std::endl;
	
	TPlanet Earth(0.000, 0.0000727, 6378100, 80000, 5.9736e24, "./Data/Images/Planets/earth.png", sf::Color(235,249,252), sf::Color(157,226,252));
	// its this pale little blue dot out there somewhere
	// rumours are that it isnt a bad place to live.	
	
	Init_assets(Earth.Get_keplerian_pointer());	
	// Create the assets used by the game
	
	while(Ignition_testing->Main_Window->window->isOpen())
	{	// open up the SFML window embedded in the ignition object
		sf::Event event;	
		// to be honest, Ive never had a clue exactly why this is inited every frame
		
		
		while (Ignition_testing->Main_Window->window->pollEvent(event))
		{	// request the main ignition window for events
			if (event.type == sf::Event::Closed)
            {	Ignition_testing->Main_Window->window->close();
				// the little x button at the top shuts this s*** down
			}
			if(event.type == sf::Event::KeyPressed)
			{	Log_keystroke(event.key.code, Ignition_testing->commands, true);
				// way to complicated to explain here,
				// basically we pass info about keypresses on to the ignition engine
			}
			if(event.type == sf::Event::KeyReleased)
			{	Log_keystroke(event.key.code, Ignition_testing->commands, false);
				// same as above, just keyreleases now
			}
        }
   		if(Title_screen->finished == true)
		{	Ignition_testing->Set_aperture_scale();
			// And we have a liftoff!!! WOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO 
			Ignition_testing->Ignition();	
			// basically the above method kicks the whole show into gear
		}	
		
			
   		Ignition_testing->Main_Window->window->setSize(sf::Vector2u(Ignition_testing->Main_Window->Width, Ignition_testing->Main_Window->Height));
   		// ahh, I think this is the part where the window is forced back to its
   		// standard size as specified in the SFML window object.
   		
   		// this might be really cool to do as a smooth resizing animation so 
   		// that the center of the view slides back into place when the user
   		// tries to resize the window. Very complex, but doable later on methinks
		
		Ignition_testing->Main_Window->window->clear();
		// gotta erase the board in order to start doing stuff again
		Title_screen->Update_screen();
		Ignition_testing->Main_Window->window->draw(*(Title_screen->splash_sprite));
		Ignition_testing->Main_Window->window->draw(*(Title_screen->Title_text));
		// this is wastefull and hideous. Needs to be fixed asap
		// the title screen should be its own SFML window & such, not hitching
		// a ride with the main ignition object
		Ignition_testing->Main_Window->window->display();
		// this is important apparently...
	}	// ahh, thats weird, why are there two calls to Ignition() ?
	Exit_program();				
	// Function cleans up our pointers before exit
    return 0;						
    // I just love happy endings, dont you?
}

void key_commands::Enter()
{	Title_screen->Toggle_fade();
}	// this should be an if/else based on whether its active too

void key_commands::Space()
{	
}	// this is a bit outdated now, would probably be better to scrap eventually

void key_commands::Comma()
{	//Starfighter->Previous_vessel();
}	// move the current vessel to the previous vessel in the main vessel list.

void key_commands::Period()
{	//Starfighter->Next_vessel();
}	// move the current vessel to the next vessel in the main vessel list.

void key_commands::Up()
{	
}

void key_commands::Left()
{	
}

void key_commands::Right()
{	
}

void key_commands::Down()
{	
}	// these were completely replaced by the wasd keys
	// maybe they could be used for surface EVAs or something?...

void key_commands::Tilde()
{	
}	// used to be killrot, could be used as some sort of menu opener


// both map zoom commands should be wrapped in a logical statement based on 

void key_commands::Dash()
{	
}	// zooms out the map view by a factor of 10

void key_commands::Equal()
{	
}	// Zooms in the map view by a factor of ten

void key_commands::V()
{	
}	// switches back & forth between the map view & the real view

void key_commands::N()
{	
}

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
}	

void key_commands::Z()
{	
}

void key_commands::X()
{	
}	// This will take some time I guess
// whatever it was...

void key_commands::C()
{	
}

void key_commands::R()
{	
}	// slow down time acceleration by 10 times

void key_commands::T()
{	
}	// speed up time acceleration by 10 times

// some sort of safety lock on the time acceleration might be nice in order
// to prevent accidentally hitting the wrong one. Maybe a double press instead
// of just one? ...

void key_commands::L()
{	// again, no clue
	// think this used to be the throttle lock
}

void key_commands::I()
{	
}	// turns the feedback displays on or off

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
}	// I dont really recall why all of these calls were here




void Init_assets(CKeplerian_Object * planet)
{	// this is all realllly bad. Everything here is a mess quite frankly
	
	sf::Texture * Flags_tex;
	// main texture for any & all flag textures that the sim uses in map view

	sf::Texture * dg_tex;
// the actual texture of the dg ship itself, passed on to the object at construction itself
// specifically, would be more effective to pass sprites by value


	sf::Sprite * csa_flag_sprite; 
// individual sprites taken from the flags texture mentioned above
// should be renamed eventually


	TVessel * GL1, * GL2, *GL3;


//	CKeplerian_Object * Earth;


	
	
	
	Talkback("Initializing assets");
	
	
	Title_screen = new SFML_titlescreen("./Data/intro.png", true, 0.700, 0.900, " ", "./Data/Fonts/Stjldbl1.ttf", 252, 223, 43, 72, sf::Vector2f(280, 50));
	
	Flags_tex = new sf::Texture();
	Flags_tex->loadFromFile("./Data/Images/logos.png");
	csa_flag_sprite = new sf::Sprite(*Flags_tex);
	csa_flag_sprite->setTextureRect(sf::IntRect(0,200,200,200));
	
	dg_tex = new sf::Texture;
	dg_tex->loadFromFile("./Data/Images/dg.png");
	std::cout << "Loaded XWing_tex from file" << std::endl;
	
	std::string orbitron_fontpath = "./Data/Fonts/orbitron-light.ttf";
	
	GL1 = new DeltaGlider(6678000.00, 0.00, 0.00, 8600.00, 270.00, 0, 40000, 20600, *dg_tex, *csa_flag_sprite, "GL-01", "./Data/Images/display_panel.png", orbitron_fontpath, RK4); 
	GL2 = new DeltaGlider(6678000.00, -12.00, 0.00, 8600.00, 180, 0, 40000, 20600, *dg_tex, *csa_flag_sprite, "GL-02", "./Data/Images/display_panel.png", orbitron_fontpath, Euler1);
	GL3 = new DeltaGlider(ObjectState(Flight_state(VectorVictor::Vector2(6678000.00, -42.00), VectorVictor::Vector2(0.00, 8600.00)), Rotation_state(180, 0, 0)), 40000, 20600, *dg_tex, *csa_flag_sprite, "GL-03", "./Data/Images/display_panel.png", orbitron_fontpath, RK4);	

	Ignition_testing->Vessel_list.insert(Ignition_testing->Vessel_list.end(), GL3);
	Ignition_testing->Vessel_list.insert(Ignition_testing->Vessel_list.end(), GL2);
	Ignition_testing->Vessel_list.insert(Ignition_testing->Vessel_list.end(), GL1);
	
	Ignition_testing->Newtonian_list.insert(Ignition_testing->Newtonian_list.end(), GL3->Get_Newtonian_pointer());	
	Ignition_testing->Newtonian_list.insert(Ignition_testing->Newtonian_list.end(), GL2->Get_Newtonian_pointer());
	Ignition_testing->Newtonian_list.insert(Ignition_testing->Newtonian_list.end(), GL1->Get_Newtonian_pointer());	
	
	Ignition_testing->Celestial_list.insert(Ignition_testing->Celestial_list.end(), planet);
	std::cout << "Finished constructing Vessels" << std::endl;
	std::cout << "Initializing text displays" << std::endl;
	std::cout << "Finished initializing text displays" << std::endl;
	std::cout << "Vessel list size " << Ignition_testing->Vessel_list.size() << " Newtonian list size " << Ignition_testing->Newtonian_list.size() << std::endl;
}

void Exit_program()
{	std::cout << "calling Exit_program()" << std::endl;
	delete Title_screen;
	//for(std::vector<TVessel*>::iterator it = Ignition_testing->Vessel_list.begin(); it != Ignition_testing->Vessel_list.end(); ++it)
	//{	delete (*it);
	//}	
	// ahh, this must be getting done in the destructor for ignition_engine
	// the windows version of the project was crashing after this was called cause
	// we were trying to delete stuff twice
	//delete dg_tex;
	//delete csa_flag_sprite;
	//delete Flags_tex;
	// bear with me, I know what I am doing
	std::cout << "Deleting the ignition object" << std::endl;
	delete Ignition_testing;
}

