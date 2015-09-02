## ignition_object.py ##########################################################
## any type of object in ignition that has a position and ######################
## possibly an image ###########################################################
################################################################################
from vector_X import *
import pygame

class ignitionObject(object):
	
	def __init__(self, position, name):
		self.Position = position
		self.ignitionName = name
	
	def getPosition(self):
		return self.Position
		
	def getIgnitionName(self):
		return self.ignitionName
