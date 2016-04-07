## graphics.py #################################################################
## graphics object, handles all drawing of game objects ########################
################################################################################
from drawable import *

##screen.blit(agent.sprite, 0, 0, agent.sprite.width, agent.sprite.height, agent.posX, agent.posY, True)



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
		
		##drawOffset = pygame.Vector2(offset.x, offset.y)
		
		if(getCurrentSystem() == "linux"):
			drawRect = image.get_rect(center=(offset.x, offset.y))
			## the problem with this is that the image doesnt necessarily need to
			## be centered on its geometric centre
			## one problem at a time though
			image.set_colorkey((0,0,0))
			screen.blit(image, drawRect)	
		elif(getCurrentSystem() == "psp"):
			screen.blit(image, 0, 0, image.width, image.height, offset.x, offset.y, True)
		
		#print "Vessel %s drawn to screen coordinates %s" % (name, drawPos)
		
		return
		
	def drawMarker(self, screen, position, colour, window_height, window_width):
		offset = position - self.Position
		offset.rotateThis(-degreesToRadians(self.Rotation))
		offset = offset + vector_II(float(window_width/2), float(window_height/2))
		
		drawPos = (int(offset.x), int(offset.y))
		drawCircle(screen, colour, drawPos)
		
	def drawBackground(self, screen, background_image, window_height, window_width):
		if(getCurrentSystem() == "linux"):
			drawRect = background_image.get_rect(center=(window_width/2, window_height/2))
			screen.blit(background_image, drawRect)
		elif(getCurrentSystem() == "psp"):
			center_x = window_width/2
			center_y = window_height/2
			screen.blit(background_image, 0, 0, background_image.width, background_image.height, 0, 0, True)		
		
		
