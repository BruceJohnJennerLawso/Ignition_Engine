## ignition_engine.py ##########################################################
## core ignition engine code ###################################################
################################################################################
from vessel import *
import sys


class Ignition_Camera:
	def __init__(self, position, rotation):
		self.Position = position
		self.Rotation = rotation
		
	def getCameraPosition(self):
		return self.Position
	
	def getCameraRotation(self):
		return self.Rotation
		
	def Update(self, deltat, target, window_height, window_width):
		self.Rotation = target.getRotation()
		offset = vector_II((-window_width),(window_height)).rotateThis(self.Rotation)
		self.Position = (target.getPosition() + offset )
		return

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
		
		self.mainCamera = Ignition_Camera(vector_II(0,0), 0.0)
		
		self.Vessels = []
		self.Vessels.insert(0, Vessel( vector_II(100, 20), vector_II(2, 0), 22, 0, "Almighty Probe", "almighty.jpg"))
	
		self.cameraTarget = self.Vessels[0];
		
	def Ignition(self):
		pygame.init()
		size = width, height = (self.windowWidth, self.windowHeight)
		black = 0, 0, 0
		screen = pygame.display.set_mode(size)
		clock = pygame.time.Clock()
		while 1:
			clock.tick()
			deltat = clock.get_time()
			print "Deltat is %f" % deltat
			clock.tick(60)
			for event in pygame.event.get():
				if event.type == pygame.QUIT: 
					#sys.exit()
					return
			for v in self.Vessels:
				v.Update(deltat)
			
			screen.fill(black)
			
			self.mainCamera.Update(deltat, self.cameraTarget, self.windowHeight, self.windowWidth)
			
			for v in self.Vessels:				
				drawPos = (int(v.getPosition().x), int(v.getPosition().y), 154, 88)
				screen.blit(v.getImage(), drawPos )				
			pygame.display.flip()

if (__name__=="__main__"):
	pyignition = Ignition_Engine(0.1, "Ignition Engine", 600, 1000)
	pyignition.Ignition();

