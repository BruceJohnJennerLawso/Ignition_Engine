## drawable.py #################################################################
## types of ingame objects that we can draw ####################################
################################################################################
from ignition_object import *

def loadImage(image):
	if(getCurrentSystem() == "linux"):
		return pygame.image.load(image)
		
	elif(getCurrentSystem() == "psp"):
		return psp2d.Image(image)

class drawableObject(ignitionObject):
	def __init__(self, position, name, image):
		super(drawableObject, self).__init__(position, name)
		self.Image = loadImage(image)


	def getImage(self):
		return self.Image;
		
		

def drawCircle(screen, colour, drawPos):
	if(getCurrentSystem() == "linux"):
		pygame.draw.circle(screen, colour, drawPos , 5, 5)
		
	elif(getCurrentSystem() == "psp"):
		print "well fuck"
		## well fuck



