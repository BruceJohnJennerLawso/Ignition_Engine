## vessel.py.py ################################################################
## generic vessel type for pyignition ##########################################
################################################################################
from newtonian import *




class Vessel(newtonianObject):
	def __init__(self, position, velocity, rotation,omega, name, image):
		super(Vessel, self).__init__(position, velocity, rotation, omega, name, image)
