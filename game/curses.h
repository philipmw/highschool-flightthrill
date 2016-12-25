// License: GNU General Public License v2

#ifndef _CURS_H_
#define _CURS_H_

#include <vector>
#include "copter.h"
#include "terrain.h"
#include "trail.h"

// Preconditions: none
// Postconditions: ncurses mode is closed and the game is exited with errorcode 0
void finish(int);

// Preconditions: none
// Postconditions: ncurses mode is initialized
void init_curses(const unsigned int);

// Preconditions: none
// Postconditions: none
// Purpose: provides a stable interface to the keyboard, in case getch() ever changes
// Return values: whatever getch() returns, at this time
int get_character();

// Preconditions: none
// Postconditions: none
// Purpose: determines whether the helicopter is still in open space or has reached an illegal area of the screen
// Return values: 0 - helicopter is still alive
//                1 - helicopter has hit a solid
//                2 - helicopter's Y coordinate is less than 0
//                3 - helicopter's Y coordinate is greater than terrain height
//                4 - helicopter's X coordinate is less than 0
//                5 - helicopter's X coordinate is greater than terrain width
int query_life_status(const Copter &, const unsigned int, const unsigned int);

// Preconditions: none
// Postconditions: the ncurses screen is redrawn with the cursor in the top-left corner
// Purpose: makes sure that the blinking cursor is not in the way of anything
void render_screen();

// Preconditions: terrain_pos does not exceed the size of the terrain structure array
// Postconditions: ncurses screen buffer contains the background
// Purpose: scans the terrain information table and prepares it for screen rendering
void draw_background(const vector <Step>, const int, const unsigned int, const unsigned int);

// Preconditions: terrain_pos does not exceed the size of the terrain structure array
// Postconditions: ncurses screen buffer contains the unactivated shields
// Purpose: scans the terrain information table and prepares it for screen rendering
void draw_shields(const vector <Step>, const int, const unsigned int, const unsigned int);

// Preconditions: none
// Postconditions: ncurses buffer contains trails
void draw_trails(const unsigned int, const Copter &, const Trail &);

// Preconditions: none
// Postconditions: ncurses screen buffer contains the helicopter
void draw_copter(const Copter &);

// Preconditions: y_pos does not exceed window height
// Postconditions: ncurses screen buffer contains a status bar
void draw_statusbar(const string, const int, const int);

// Preconditions: sgm_pixels is non-negative
// Postconditions: copter's Y coordinate is changed to where the copter ended up during its fall
// Purpose: to shock players with blood and gore as the burning and mangled helicopter falls, accelerating, and explodes when it reaches the bottom.
void plunge_copter(Copter &, const unsigned int, const unsigned int, const unsigned int, const TerrainMetadata);

#endif
