## ignition_engine.py ##########################################################
## core ignition engine code ###################################################
################################################################################
from graphics import *
import sys

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
		self.Vessels.insert(0, Vessel( vector_II(100, -20), vector_II(0.001, 0), 0, 0.0005, "Almighty Probe", "almighty.jpg"))
		self.Vessels.insert(0, Vessel( vector_II(100, -100), vector_II(0, 0), 0, 0.0001, "Almighty Probe2", "almighty.jpg"))		
	
		self.Markers = []
		blue = 0,0,100
		self.Markers.insert(0, ignitionMarker(vector_II(100,-150), "foo marker", blue ) )
	
		self.cameraTarget = self.Vessels[0];
		
		self.Starfield = pygame.image.load("./Images/starfield.png")
		
	def endSimulation(self, frame_cutoff):
		if(frame_cutoff != 0):
			if(frame_cutoff <= 0):
				return True;
		return False;
		
	def decreaseTimeAcceleration(self):
		if(self.timeAcceleration > 1):
			self.timeAcceleration -= 1	
		
	def increaseTimeAcceleration(self):
		self.timeAcceleration += 1
		
	def Ignition(self, frame_cutoff = 0):
		pygame.init()
		size = width, height = (self.windowWidth, self.windowHeight)
		black = 0, 0, 0
		screen = pygame.display.set_mode(size)
		clock = pygame.time.Clock()
		while (self.endSimulation(frame_cutoff) != True):
			
			for event in pygame.event.get():
				if(event.type == pygame.QUIT):
					frame_cutoff = 0.5
				if(event.type == pygame.KEYDOWN):
					if(event.key == pygame.K_t):
						self.increaseTimeAcceleration()
					elif(event.key == pygame.K_r):
						self.decreaseTimeAcceleration()	
			
			
			
			clock.tick()
			deltat = clock.get_time()
			if(deltat != 0):
				frameRate = 1000/deltat
			else:
				frameRate = 0
			print "Deltat is %d ms, framerate is %dfps, timeAcceleration is %d frame cutoff is %d frames" % (deltat, int(frameRate), int(self.timeAcceleration), frame_cutoff)
			clock.tick(60)
			
			if(self.timeAcceleration != 0):
				deltat = deltat * math.pow(10, self.timeAcceleration)
			
			for v in self.Vessels:
				v.Update(deltat)
			
			screen.fill(black)
			
			self.mainCamera.Update(deltat, self.cameraTarget, self.windowHeight, self.windowWidth)
			self.mainCamera.drawBackground(screen, pygame.transform.rotate(self.Starfield, self.mainCamera.Rotation), self.windowHeight, self.windowWidth)
			for v in self.Vessels:				
				self.mainCamera.drawTo(screen, pygame.transform.rotate(v.getImage(), -(v.getRotation()- self.mainCamera.Rotation)), v.getIgnitionName(), v.getPosition(), v.getRotation(), self.windowHeight, self.windowWidth)  
			for v in self.Vessels:				
				green = 0,255,0
				vesMarker = ignitionMarker(v.getPosition(), v.getIgnitionName()+ (" Marker"), green )
				
				self.mainCamera.drawMarker(screen, vesMarker.getPosition(), vesMarker.getMarkerColour(), self.windowHeight, self.windowWidth)  
				#v.printNewtonianInfo()	
			for m in self.Markers:
				self.mainCamera.drawMarker(screen, m.getPosition(), m.getMarkerColour(), self.windowHeight, self.windowWidth)  
			pygame.display.flip()
			if(frame_cutoff != 0):
				frame_cutoff -= 1;
			sys.stdout.write("\x1b[2J\x1b[H");

if (__name__=="__main__"):
	pyignition = Ignition_Engine(0.1, "Ignition Engine", 600, 1000)
	pyignition.Ignition();

