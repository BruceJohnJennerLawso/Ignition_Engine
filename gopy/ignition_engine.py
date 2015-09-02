## ignition_engine.py ##########################################################
## core ignition engine code ###################################################
################################################################################
from vessel import *
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
		
		
		self.Vessels = []
		self.Vessels.insert(0, Vessel( vector_II(100, 20), vector_II(2, 0), 22, 0, "Almighty Probe", "almighty.jpg"))
	
	def Ignition(self):
		pygame.init()
		size = width, height = (self.windowWidth, self.windowHeight)
		speed = [1, 1]
		black = 0, 0, 0
		
		screen = pygame.display.set_mode(size)
		
		ball = pygame.image.load("almighty.jpg")
		
		clock = pygame.time.Clock()
		
		ballrect = ball.get_rect()
		print ballrect
		while 1:
			deltat = clock.get_time()
			clock.tick(60)
			for event in pygame.event.get():
				if event.type == pygame.QUIT: 
					#sys.exit()
					return
			for v in self.Vessels:
				v.Update(deltat)
			
			screen.fill(black)
			
			for v in self.Vessels:
				print
				drawPos = (int(v.getPosition().x), int(v.getPosition().y), 154, 88)
				print drawPos
				#screen.blit(ball, drawPos )
				screen.blit(v.getImage(), drawPos )				
				
			#ballrect = ballrect.move(speed)
			#if ballrect.left < 0 or ballrect.right > width:
			#	speed[0] = -speed[0]
			#if ballrect.top < 0 or ballrect.bottom > height:
			#	speed[1] = -speed[1]
			#screen.blit(ball, ballrect)
			pygame.display.flip()

if (__name__=="__main__"):
	pyignition = Ignition_Engine(0.1, "Ignition Engine", 600, 1000)
	pyignition.Ignition();

