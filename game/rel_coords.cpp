// License: GNU General Public License v2

#include "generic.h"
#include "rel_coords.h"

int percent_to_pixels(const int percentage, const int screen_size)
{
	return double_to_int(double(screen_size*percentage)/100);
}

int pixels_to_percent(const int pixels, const int screen_size)
{
	return double_to_int(double(pixels*100)/screen_size);
}
