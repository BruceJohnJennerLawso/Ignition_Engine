## script.py ###################################################################
## default run script on the psp ###############################################
################################################################################
from ignition_engine import *

if (__name__=="__main__"):
	
	pyignition = Ignition_Engine(0.1, "Ignition Engine", 272, 480)
	pyignition.Ignition();
