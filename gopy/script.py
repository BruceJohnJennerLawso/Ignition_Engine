## script.py ###################################################################
## default run script on the psp ###############################################
################################################################################
import psp2d
from ignition_engine import *

if (__name__=="__main__"):
	font = psp2d.Font("font.png")
	image = psp2d.Image(480, 272)
	screen = psp2d.Screen()
	CLEAR_COLOR = psp2d.Color(0,0,0)
	image.clear(CLEAR_COLOR)
	font.drawText(image, 0, 0, "Ignition Engine")
	screen.blit(image)
	screen.swap()
	
	
	pyignition = Ignition_Engine(0.1, "Ignition Engine", 272, 480)
	pyignition.Ignition();
