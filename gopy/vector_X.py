## vector.py ###################################################################
## vector3 class, first module of the project ##################################
################################################################################
import math


def degreesToRadians(degrees):
	output = ((degrees*2*math.pi)/360)
	return output
	
def radiansToDegrees(radians):
	output = ((radians*360)/(2*math.pi))
	return output


class vector_II:
	"""2 dimensional vector class in python"""
	def __init__(self, x, y):
		self.x = x
		self.y = y
	
		return
	
	def getVec(self):
		output = "(%f, %f)\n" % (self.x, self.y)
		return output;
	
	def printVec(self): 
		print self.getVec();
		return
		
	def __add__(self, b):
		output = vector_II(self.x, self.y)
		output.x = (self.x + b.x)
		output.y = (self.y + b.y)
		return output
		
	def __sub__(self, b):
		output = vector_II(self.x, self.y)
		output.x = (self.x - b.x)
		output.y = (self.y - b.y)
		return output		
		
	
	def __mul__(self, value):
		output = vector_II(self.x, self.y)
		output.x *= value
		output.y *= value
		return output
		
	def Dot(self, vec):
		output = ((self.x*vec.x)+(self.y*vec.y));
		return output;
		
	def Rotate(self, rotation):
		""" Return a rotated version of this vector, rotated
		by rotation radians clockwise. Does not modify the
		original vector """
		output = vector_II(0,0)
		
		output.x = ( (self.x*(math.cos(rotation)))+(self.y*(math.sin(rotation))) )
		output.y = ( (self.x*((-1)*math.sin(rotation)))+(self.y*(math.cos(rotation))) )
		
		return output
		
		
	def rotateThis(self, rotation):
		""" Rotate this vector by rotation radians clockwise.
		Does modify the original vector """
		new_x = ( (self.x*(math.cos(rotation)))+(self.y*(math.sin(rotation))) )
		self.y = ( (self.x*((-1)*math.sin(rotation)))+(self.y*(math.cos(rotation))) )
		self.x = new_x
		return
		
	def This(self):
		return self;
		# always unto thyself return?

if (__name__=="__main__"):
	v = vector_II(2, 2)
	w = vector_II(1, -1)
	print "v: %sw: %s" % (v.getVec(), w.getVec())
	z = v + w.This()
	print "v: %sw: %sz: %s" % (v.getVec(), w.getVec(), z.getVec())
	
	z = v*3
	print "v: %sw: %sz: %s" % (v.getVec(), w.getVec(), z.getVec())
	print v.x
