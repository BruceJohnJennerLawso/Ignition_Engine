// mfd.hpp /////////////////////////////////////////////////////////////////////
//  MFD Multifunctional Displays, so we dont have to fly blind anymore /////////
////////////////////////////////////////////////////////////////////////////////
//#include <std_files>
//#include "Headers.h"
//#include "Headers.hpp"
//#include "Source.cpp"
#include "Vessel.hpp"

class MFD
{	public:
	MFD();
	virtual void Render_MFD(TVessel * vessah);
	~MFD();
};
