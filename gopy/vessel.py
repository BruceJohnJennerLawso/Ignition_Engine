## vessel.py.py ################################################################
## generic vessel type for pyignition ##########################################
################################################################################
from newtonian import *

class vesselPart:
	def __init__(self, vessel_position, 



class Vessel(newtonianObject):
	def __init__(self, position, velocity, rotation, name):
		super(Vessel, self).__init__(position, velocity, rotation, name)
