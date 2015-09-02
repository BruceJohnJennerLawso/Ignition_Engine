## newtonian.py.py #############################################################
## generic newtonian object type in ignition ###################################
################################################################################
from ignition_object import *

class newtonianObject(ignitionObject):
	def __init__(self, position, velocity, rotation, omega, name):
		super(newtonianObject, self).__init__(position, name)
		
		self.Velocity = velocity
		
		self.Rotation = rotation
		self.Omega = omega
	
	def getVelocity(self):
		return self.Velocity
		
	def getRotation(self):
		return self.Rotation

	def getOmega(self):
		return self.Omega

	def updateRotation(self, deltat):
		self.Rotation += (self.Omega*deltat)
		return
		
	def updatePosition(self, deltat):
		self.Position += (self.Velocity*deltat)
		
	def Update(self, deltat):
		self.updateRotation(deltat)
		self.updatePosition(deltat)
