## ignition_engine.py ##########################################################
## core ignition engine code ###################################################
################################################################################
from vessel import *
import sys
import pygame


class Vessel:
	def __init__(self, mass, init_pos, init_rot):
		self.Mass = mass;
		self.Position = init_pos
		self.Rotation = init_rot

	def printAttributes(self):
		print "Mass %f, at %s, rotation %f" % (self.Mass, self.Position.getVec(), self.Rotation);

#class almightyProbe

class Ignition_Engine:
	def __init__(self, redraw_interval, window_title, window_height, window_width):
		self.redrawInterval = redraw_interval
		self.windowTitle = window_title
		self.cameraScale = 1.0
		self.timeAcceleration = 1
		self.simulationTime = 0.0
		self.displaysActive = True
		self.windowHeight = window_height
		self.windowWidth = window_width
		
		
		self.Vessels = []
	
	def Ignition(self):
		pygame.init()
		size = width, height = (self.windowWidth, self.windowHeight)
		speed = [1, 1]
		black = 0, 0, 0
		
		screen = pygame.display.set_mode(size)
		
		ball = pygame.image.load("almighty.jpg")
		
		ballrect = ball.get_rect()
		while 1:
			for event in pygame.event.get():
				if event.type == pygame.QUIT: 
					#sys.exit()
					return
			ballrect = ballrect.move(speed)
			if ballrect.left < 0 or ballrect.right > width:
				speed[0] = -speed[0]
			if ballrect.top < 0 or ballrect.bottom > height:
				speed[1] = -speed[1]
			screen.fill(black)
			screen.blit(ball, ballrect)
			pygame.display.flip()

if (__name__=="__main__"):
	pyignition = Ignition_Engine(0.1, "Ignition Engine", 200, 200)
	pyignition.Ignition();

