## ignition_engine.py ##########################################################
## core ignition engine code ###################################################
################################################################################
from vessel import *
import sys


class Ignition_Camera:
	def __init__(self, position, rotation, scale):
		self.Position = position
		self.Rotation = rotation
		self.Scale = scale
		
	def getCameraPosition(self):
		return self.Position
	
	def getCameraRotation(self):
		return self.Rotation
		
	def Update(self, deltat, target, window_height, window_width):
		self.Rotation = target.getRotation()
		self.Position = target.getPosition()
		return
	
	def drawTo(self, screen, image, name, position, rotation, window_height, window_width):

		offset = position - self.Position
		offset.rotateThis(-degreesToRadians(self.Rotation))
		offset = offset + vector_II(float(window_width/2), float(window_height/2))
		
		drawPos = (offset.x, offset.y)
		# needs ability to rotate images being drawn to screen
		screen.blit(image, drawPos)	
		print "Vessel %s drawn to screen coordinates %s" % (name, drawPos)
		
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
		
		self.mainCamera = Ignition_Camera(vector_II(0,0), 0.0, 1.0)
		
		self.Vessels = []
		self.Vessels.insert(0, Vessel( vector_II(100, -20), vector_II(0.1, 0), 0, 0, "Almighty Probe", "almighty.jpg"))
		self.Vessels.insert(0, Vessel( vector_II(100, -100), vector_II(0, 0), 0, 0, "Almighty Probe2", "almighty.jpg"))		
	
		self.cameraTarget = self.Vessels[0];
		
	def endSimulation(self, frame_cutoff):
		if(frame_cutoff != 0):
			if(frame_cutoff <= 0):
				return True;
		return False;
		
	def Ignition(self, frame_cutoff = 0):
		pygame.init()
		size = width, height = (self.windowWidth, self.windowHeight)
		black = 0, 0, 0
		screen = pygame.display.set_mode(size)
		clock = pygame.time.Clock()
		while (self.endSimulation(frame_cutoff) != True):
			clock.tick()
			deltat = clock.get_time()
			if(deltat != 0):
				frameRate = 1000/deltat
			else:
				frameRate = 0
			print "Deltat is %f, framerate is %f, frame cutoff is %s" % (deltat, frameRate, frame_cutoff)
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
				self.mainCamera.drawTo(screen, v.getImage(), v.getIgnitionName(), v.getPosition(), v.getRotation(), self.windowHeight, self.windowWidth)  
				v.printNewtonianInfo()
			pygame.display.flip()
			if(frame_cutoff != 0):
				frame_cutoff -= 1;
			#sys.stdout.write("\x1b[2J\x1b[H");

if (__name__=="__main__"):
	pyignition = Ignition_Engine(0.1, "Ignition Engine", 600, 1000)
	pyignition.Ignition();

