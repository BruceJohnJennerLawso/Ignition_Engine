## systemDetect.py #############################################################
## trying to get around this bizarre issue with the way that pygame runs #######
## on psp ######################################################################
################################################################################


def getCurrentSystem(i=0):
	if(i == 0):
		import pygame
		## assume linux desktop, nothing weird here
		return "linux"
	elif(i == 1):
		return "headless"
	elif(i == 2):
		import psp2d
		## psp weirdness
		return "psp"
	else:
		return "unknown"

getCurrentSystem()
## call this off the top, so we can make sure the import is done right away
