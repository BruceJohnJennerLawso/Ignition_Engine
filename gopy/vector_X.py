## vector.py ###################################################################
## vector3 class, first module of the project ##################################
################################################################################
import math
##from systemDetect import *

##import psp2d
import pygame

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
## bizarre issue with Python 2.5.2, where it refuses to work with the
## systemDetect file due to time of modification issues or something


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

def additionTest(inputVector):
	bar = vector_II(9,9)
	foo = bar + inputVector
	print "foo: %sbar: %sinput: %s" % (foo.getVec(), bar.getVec(), inputVector.getVec())

if (__name__=="__main__"):
	v = vector_II(2, 2)
	w = vector_II(1, -1)
	print "v: %sw: %s" % (v.getVec(), w.getVec())
	z = v + w.This()
	print "v: %sw: %sz: %s" % (v.getVec(), w.getVec(), z.getVec())
	
	z = v*3
	print "v: %sw: %sz: %s" % (v.getVec(), w.getVec(), z.getVec())
	print v.x
	
	additionTest(z)
	
	a = vector_II(1,1)
	
	b = a.Rotate(degreesToRadians(45))
	print "vector a: %svector b: %s rotated 45 degrees" % (a.getVec(), b.getVec())
	b.rotateThis(degreesToRadians(45))
	print "vector b %s rotated another 45 degrees" % b.getVec()
	
	
