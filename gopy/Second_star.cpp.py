# Second star to the right ##########################
# And straight on till we hit the precision limits of long double ######/
########################################

#include "Ignition_Engine.hpp"
#define  Ignition_version " 0.20"	# see? progress ;)
# self should be renamed to reflect the move away from Starfighter


# Engine Pointers ##############################/
######################################/

# these pointers really need to be reduced to bare minimum if possible

# EVIL EVIL IS ERRYWHERE

Ignition_engine * Ignition_testing
# the main ignition engine object


SFML_titlescreen * Title_screen
# the splash screen before the program really gets goings 



#template <class T> void Iterate_forward(std.vector<T> &target_vector, it, &target_value)		# References are needed for the second pass, to have for the first
#{	#std.vector<T>.iterator it
#	unsigned cy = 0
#	for(it = target_vector.begin(); it != target_vector.end(); ++it)
#	{	if (*it) == target_value:
#		{	if it == target_vector.begin():
#			{	target_value = target_vector.at(target_vector.size() - 1)
#
#			elif it > target_vector.begin():
#			{	target_value = target_vector.at(cy - 1)
#
#			break
#
#		cy++
#
#

# self seemed legit, I got a weird issue when trying to compile :(



######################################

def Intro_splash_screen(self):
def Update_audio(self):
# havent a clue what these do. Better find out


def Set_current_vessel(self, * target_vessel):
# augh mine eyes are bleeding, whyyyyyyy



# 0	frame rate
# 1	map scale
# 2	time acceleration factor
# 3	simulation time
# 4
# 5

# Ahh, can go, relevant anymore


def main(self):
	Window_title = "Ignition Engine"
	# gotta give our baby a name
	Window_title.append(Ignition_version)
	# stick the version numbah on the end
	std.cout << "Constructing Ignition Engine" << std.endl
	Ignition_testing = Ignition_engine(Window_title, 609, 1024, Ignition_version, 0.2, "./Data/Fonts/orbitron-light.ttf", "./Data/Images/starfield.png");	
	# give birth to our beautiful engine object. Isnt it cute?
	std.cout << "Right before Init_assets()" << std.endl
	
	TPlanet Earth(0.000, 0.0000727, 6378100, 80000, 5.9736e24, "./Data/Images/Planets/earth.png", sf.Color(235,249,252), sf.Color(157,226,252))
	# its self pale little blue dot out there somewhere
	# rumours are that it isnt a bad place to live.	
	
	Init_assets(Earth.Get_keplerian_pointer());	
	# Create the assets used by the game
	
	while(Ignition_testing.Main_Window.window.isOpen())
	{	# open up the SFML window embedded in the ignition object
		sf.Event event;	
		# to be honest, never had a clue exactly why self is inited every frame
		
		
		while (Ignition_testing.Main_Window.window.pollEvent(event))
		{	# request the main ignition window for events
			if event.type == sf.Event.Closed:
            {	Ignition_testing.Main_Window.window.close()
				# the little x button at the top shuts self s*** down

			if event.type == sf.Event.KeyPressed:
			{	Log_keystroke(event.key.code, Ignition_testing.commands, True)
				# way to complicated to explain here,
				# basically we pass info about keypresses on to the ignition engine

			if event.type == sf.Event.KeyReleased:
			{	Log_keystroke(event.key.code, Ignition_testing.commands, False)
				# same as above, keyreleases now


   		if Title_screen.finished == True:
		{	Ignition_testing.Set_aperture_scale()
			# And we have a liftoffnot !not  WOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO 
			Ignition_testing.Ignition();	
			# basically the above method kicks the whole show into gear
		}	
		
			
   		Ignition_testing.Main_Window.window.setSize(sf.Vector2u(Ignition_testing.Main_Window.Width, Ignition_testing.Main_Window.Height))
   		# ahh, think self is the part where the window is forced back to its
   		# standard size as specified in the SFML window object.
   		
   		# self might be really cool to do as a smooth resizing animation so 
   		# that the center of the view slides back into place when the user
   		# tries to resize the window. Very complex, doable later on methinks
		
		Ignition_testing.Main_Window.window.clear()
		# gotta erase the board in order to start doing stuff again
		Title_screen.Update_screen()
		Ignition_testing.Main_Window.window.draw(*(Title_screen.splash_sprite))
		Ignition_testing.Main_Window.window.draw(*(Title_screen.Title_text))
		# self is wastefull and hideous. Needs to be fixed asap
		# the title screen should be its own SFML window & such, hitching
		# a ride with the main ignition object
		Ignition_testing.Main_Window.window.display()
		# self is important apparently...
	}	# ahh, weird, are there two calls to Ignition() ?
	Exit_program();				
	# Function cleans up our pointers before exit
    return 0;						
    # I just love happy endings, you?


def Enter(self):
{	Title_screen.Toggle_fade()
}	# self should be an if/else based on whether its active too

def Space(self):
{	
}	# self is a bit outdated now, probably be better to scrap eventually

def Comma(self):
{	#Starfighter.Previous_vessel()
}	# move the current vessel to the previous vessel in the main vessel list.

def Period(self):
{	#Starfighter.Next_vessel()
}	# move the current vessel to the next vessel in the main vessel list.

def Up(self):
{	


def Left(self):
{	


def Right(self):
{	


def Down(self):
{	
}	# these were completely replaced by the wasd keys
	# maybe they could be used for surface EVAs or something?...

def Tilde(self):
{	
}	# used to be killrot, be used as some sort of menu opener


# both map zoom commands should be wrapped in a logical statement based on 

def Dash(self):
{	
}	# zooms out the map view by a factor of 10

def Equal(self):
{	
}	# Zooms in the map view by a factor of ten

def V(self):
{	
}	# switches back & forth between the map view & the real view

def N(self):
{	


def Q(self):
{	


def W(self):
{	


def E(self):
{	


def A(self):
{	


def S(self):
{	


def D(self):
{	


def F(self):
{	
}	

def Z(self):
{	


def X(self):
{	
}	# This will take some time I guess
# whatever it was...

def C(self):
{	


def R(self):
{	
}	# slow down time acceleration by 10 times

def T(self):
{	
}	# speed up time acceleration by 10 times

# some sort of safety lock on the time acceleration might be nice in order
# to prevent accidentally hitting the wrong one. Maybe a double press instead
# of just one? ...

def L(self):
{	# again, clue
	# think self used to be the throttle lock


def I(self):
{	
}	# turns the feedback displays on or off

def Numpad_0(self):
{	std.cout << "Numpad_0() called" << std.endl


def Numpad_1(self):
{	std.cout << "Numpad_1() called" << std.endl


def Numpad_2(self):
{	std.cout << "Numpad_2() called" << std.endl


def Numpad_3(self):
{	std.cout << "Numpad_3() called" << std.endl


def Numpad_4(self):
{	std.cout << "Numpad_4() called" << std.endl


def Numpad_5(self):
{	std.cout << "Numpad_5() called" << std.endl


def Numpad_6(self):
{	std.cout << "Numpad_6() called" << std.endl


def Numpad_7(self):
{	std.cout << "Numpad_7() called" << std.endl


def Numpad_8(self):
{	std.cout << "Numpad_8() called" << std.endl


def Numpad_9(self):
{	std.cout << "Numpad_9() called" << std.endl


def Plus(self):
{	std.cout << "Plus() called" << std.endl


def Minus(self):
{	std.cout << "Minus() called" << std.endl
}	# I dont really recall why all of these calls were here




def Init_assets(self, * planet):
{	# self is all realllly bad. Everything here is a mess quite frankly
	
	sf.Texture * Flags_tex
	# main texture for any & all flag textures that the sim uses in map view

	sf.Texture * dg_tex
# the actual texture of the dg ship itself, on to the object at construction itself
# specifically, be more effective to pass sprites by value


	sf.Sprite * csa_flag_sprite; 
# individual sprites taken from the flags texture mentioned above
# should be renamed eventually


	TVessel * GL1, GL2, *GL3


#	CKeplerian_Object * Earth


	
	
	
	Talkback("Initializing assets")
	
	
	Title_screen = SFML_titlescreen("./Data/intro.png", True, 0.700, 0.900, " ", "./Data/Fonts/Stjldbl1.ttf", 252, 223, 43, 72, sf.Vector2f(280, 50))
	
	Flags_tex = sf.Texture()
	Flags_tex.loadFromFile("./Data/Images/logos.png")
	csa_flag_sprite = sf.Sprite(*Flags_tex)
	csa_flag_sprite.setTextureRect(sf.IntRect(0,200,200,200))
	
	dg_tex = sf.Texture
	dg_tex.loadFromFile("./Data/Images/dg.png")
	std.cout << "Loaded dg_tex from file" << std.endl
	
	orbitron_fontpath = "./Data/Fonts/orbitron-light.ttf"
	
	GL1 = DeltaGlider(6678000.00, 0.00, 0.00, 8600.00, 270.00, 0, 40000, 20600, *dg_tex, *csa_flag_sprite, "GL-01", "./Data/Images/display_panel.png", orbitron_fontpath, RK4); 
	GL2 = DeltaGlider(6678000.00, -12.00, 0.00, 8600.00, 180, 0, 40000, 20600, *dg_tex, *csa_flag_sprite, "GL-02", "./Data/Images/display_panel.png", orbitron_fontpath, Euler1)
	GL3 = DeltaGlider(ObjectState(Flight_state(VectorVictor.Vector2(6678000.00, -42.00), VectorVictor.Vector2(0.00, 8600.00)), Rotation_state(180, 0, 0)), 40000, 20600, *dg_tex, *csa_flag_sprite, "GL-03", "./Data/Images/display_panel.png", orbitron_fontpath, RK4);	
	
	Create_deltaglider(Ignition_testing.Vessel_list, Ignition_testing.Newtonian_list, Construct_deltaglider(ObjectState(Flight_state(VectorVictor.Vector2(6678000.00, -42.00), VectorVictor.Vector2(0.00, 6800.00)), Rotation_state(0, 0, 0)), 40000, 20600, *dg_tex, *csa_flag_sprite, "GL-04", "./Data/Images/display_panel.png", orbitron_fontpath, RK4))
	# works like a charm. One more spike in the coffin

	Ignition_testing.Vessel_list.insert(Ignition_testing.Vessel_list.end(), GL3)
	Ignition_testing.Vessel_list.insert(Ignition_testing.Vessel_list.end(), GL2)
	Ignition_testing.Vessel_list.insert(Ignition_testing.Vessel_list.end(), GL1)
	
	Ignition_testing.Newtonian_list.insert(Ignition_testing.Newtonian_list.end(), GL3.Get_Newtonian_pointer());	
	Ignition_testing.Newtonian_list.insert(Ignition_testing.Newtonian_list.end(), GL2.Get_Newtonian_pointer())
	Ignition_testing.Newtonian_list.insert(Ignition_testing.Newtonian_list.end(), GL1.Get_Newtonian_pointer());	
	
	Ignition_testing.Celestial_list.insert(Ignition_testing.Celestial_list.end(), planet)
	std.cout << "Finished constructing Vessels" << std.endl
	std.cout << "Initializing text displays" << std.endl
	std.cout << "Finished initializing text displays" << std.endl
	std.cout << "Vessel list size " << Ignition_testing.Vessel_list.size() << " Newtonian list size " << Ignition_testing.Newtonian_list.size() << std.endl


def Exit_program(self):
{	std.cout << "calling Exit_program()" << std.endl
	delete Title_screen
	#for(std.vector<TVessel*>it = Ignition_testing.Vessel_list.begin(); it != Ignition_testing.Vessel_list.end(); ++it)
	#{	delete (*it)
	#}	
	# ahh, must be getting done in the destructor for ignition_engine
	# the windows version of the project was crashing after self was called cause
	# we were trying to delete stuff twice
	#delete dg_tex
	#delete csa_flag_sprite
	#delete Flags_tex
	# bear with me, know what I am doing
	std.cout << "Deleting the ignition object" << std.endl
	delete Ignition_testing


