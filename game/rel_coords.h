// License: GNU General Public License v2

#ifndef _REL_COORDS_H_
#define _REL_COORDS_H_

// Preconditions: none
// Postconditions: none
// Purpose: converts a percent measure to a true pixel measure, based on the maximum available size
int percent_to_pixels(const int, const int);

// Preconditions: none
// Postconditions: none
// Purpose: converts a pixel measure to a percentage, based on the maximum available size
int pixels_to_percent(const int, const int);

#endif
