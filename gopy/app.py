## app.py ######################################################################
## main ignition testing application ###########################################
################################################################################
import pygame
from ignition_engine import *
	
if(__name__ == "__main__"):	
	pyignition = Ignition_Engine(0.1, "Ignition Engine", 240, 320)
	pyignition.Ignition();
