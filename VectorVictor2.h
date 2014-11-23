// VectorVictor2.h /////////////////////////////////////////////////////////////
// A standard template header file for my projects /////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//#include <std_files>
//#include "Headers.h"
//#include "Source.cpp"
#include "Ignition.h"

#define Pi 	3.14159265358979323846
#define Tau 6.283185308								// Hopefully this doesnt anger the anti-tau-ans. ooh wait, the antautans! Or maybe just anti-tauntans
#define Pau 4.712388980								// anti-tauntans: smell even better on the inside!

// The VectorVictor namespace //////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

namespace VectorVictor	// Honourable mention goes to WectorWictor Two. Sorry Chekov
{	class Vector2	
	{	public:
		Vector2 ();
		Vector2 (long double ix, long double iy);
		//private:	// it really shouldnt be like this, but my patience is unbelievably low right now. The calls that are directly accessing the position need to be rewritten to use the functions at a later date
		long double x, y;
		public:
		void Set_x (long double ix);
		void Set_y (long double iy);
		void Set_values(long double ix, long double iy);
		long double Get_x();
		long double Get_y();	
		std::string Get_vector(std::string unit);
		Vector2 Vector_normal();												// confirmed working
		void Normalize();															// confirmed working
		void Rotate_vector(long double rotation);					// confirmed working
		Vector2 Get_rotated_vector(long double rotation);		// confirmed working
		long double Get_vector_magnitude();							// confirmed working
		long double Get_vector_magnitude_squared();			// probably good
		Vector2 operator + (const Vector2);								// confirmed working
		Vector2 operator += (const Vector2);							// confirmed working
		Vector2 operator - (const Vector2);								// probably good
		Vector2 operator -= (const Vector2);							// confirmed working
		Vector2 operator = (const Vector2 v);							// confirmed working
		Vector2 operator *= (const double c);							// confirmed working
		bool operator != (const Vector2 v);								// confirmed working
		bool operator == (const Vector2 v);								// confirmed working
		~Vector2(void);						
	};

	long double Get_dot_product(long double x1, long double y1, long double x2, long double y2);
	long double Get_dot_product(Vector2 First_vector, Vector2 Second_vector);
	long double Get_vector_angle(Vector2 First_vector, Vector2 Second_vector);
	long double Get_cross_product(Vector2 First_vector, Vector2 Second_vector);
	
	long double Get_vector_separation(Vector2 First_vector, Vector2 Second_vector);
};

