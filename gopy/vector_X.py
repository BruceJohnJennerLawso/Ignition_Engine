## vector.py ###################################################################
## vector3 class, first module of the project ##################################
################################################################################


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
		
	def This(self):
		return self;
		# always unto thyself return?

if (__name__=="__main__"):
	v = vector_II(2, 2)
	w = vector_II(1, -1)
	print "v: %sw: %s" % (v.getVec(), w.getVec())
	z = v + w
	print "v: %sw: %sz: %s" % (v.getVec(), w.getVec(), z.getVec())
	
	z = v*3
	print "v: %sw: %sz: %s" % (v.getVec(), w.getVec(), z.getVec())
