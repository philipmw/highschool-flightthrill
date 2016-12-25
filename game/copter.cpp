// License: GNU General Public License v2
// This is the Copter class which encapsulates 2D properties and aspects of a helicopter

#include "copter.h"
#include "generic.h"
#include "constants.h"
#include "curses.h"
#include "rel_coords.h"

Copter::Copter(int newx, int newy, int newlowx, int newlowy, int newhighx, int newhighy)
{
	// we trust the instantiator
	x_coord = newx;
	y_coord = newy;

	x_low = newlowx;
	y_low = newlowy;
	x_high = newhighx;
	y_high = newhighy;

	direction = true;
	protection = 0;
	time = 0;
}

unsigned int Copter::query_x() const
{
	return x_coord;
}

int Copter::query_y() const
{
	return y_coord;
}

unsigned int Copter::query_prot() const
{
	return protection;
}

bool Copter::set_y(int newy)
{
	y_coord = newy;
	return true;
}

bool Copter::incr_x(int mod)
{
	if (mod < 1)
		return false;
	affect_prot(-1*mod);
	x_coord += mod;
	return true;
}

bool Copter::incr_y(int mod)
{
	return set_y(y_coord+mod);
}

bool Copter::set_x_low(int val)
{
	x_low = val;
	return true;
}

bool Copter::set_x_high(int val)
{
	x_high = val;
	return true;
}

bool Copter::set_y_low(int val)
{
	y_low = val;
	return true;
}

bool Copter::set_y_high(int val)
{
	y_high = val;
	return true;
}

bool Copter::affect_prot(int val)
{
	if (int(protection)+val < 0)
		return false;
	protection += val;
	return true;
}

bool Copter::reset_prot()
{
	if (protection > 0)
	{
		protection = 0;
		return true;
	}
	return false;
}

void Copter::apply_physics(bool is_increasing, const int terrain_height, const double gravity)
{
	double acceleration=0, displacement=0;

	// `time' is how long a direction has been maintained.
	if (is_increasing == direction) // if no change in direction
		time++;
	else //change of direction
	{
		if (time > 0) // current direction opposing inertia
			time--;
		else // inertia has been overcome
			direction = is_increasing;
	}
	
	if (direction) // if ascending
		acceleration = HELICOPTER_ACCELERATION;
	else // if descending
		acceleration = gravity;
	displacement = acceleration * pow((long double)time, (long double)2) / Y_MOTION_DAMPER;
	y_coord += percent_to_pixels(double_to_int(displacement), terrain_height);
}

void Copter::reset_physics()
{
	direction = true;
	time = 0;
}
