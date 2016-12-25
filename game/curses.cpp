// License: GNU General Public License v2
// Everything here except init_curses() is supposed to be executed while in ncurses

#include <signal.h>
#include "constants.h"
#include "generic.h"
#include "curses.h"
#include "rel_coords.h"

void finish(int sig)
{
    endwin();
    cout << "The game has been terminated." << endl;
    exit(0);
}

void init_curses(const unsigned int delay_val)
{
	cout << "Initializing ncurses... please wait." << endl;
	signal(SIGINT, finish);      // arrange interrupts to terminate

	initscr();      // initialize the curses library
	keypad(stdscr, true);  // enable keyboard mapping
	nonl();         // tell curses not to do NL->CR/NL on output
	cbreak();       // take input chars one at a time, no wait for \n
	noecho();       // don't echo input
	nodelay(stdscr, true);	// non-blocking I/O
	//halfdelay(delay_val); // blocking I/O in lieu of usleep()

	if (has_colors())
	{
		start_color();

		/*
		* Simple color assignment, often all we need.
		*/
		init_pair(COLOR_BLACK, COLOR_BLACK, COLOR_BLACK);
		init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
		init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
		init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
		init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
		init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
		init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
		init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
	}
}

// This wrapper is here in case you decide to do something fancy to get keystrokes
int get_character()
{
	return getch();
}

int query_life_status(const Copter &copter, const unsigned int terrain_height, const unsigned int terrain_width)
{
	if (char(mvinch(copter.query_y(), copter.query_x()) & A_CHARTEXT) == SOLID_SYMBOL)
		return 1;
	if (copter.query_y() < 0)
		return 2;
	if (copter.query_y() >= int(terrain_height))
		return 3;
	if (copter.query_x() < 0)
		return 4;
	if (copter.query_x() >= terrain_width)
		return 5;
	return 0;
}

void render_screen()
{
	move(0, 0);
	refresh();
}

void draw_background(const vector <Step> terrain, const int terrain_pos, const unsigned int terrain_height, const unsigned int terrain_width)
{
	// terrain_pos is the pointer to the LAST (right-most) element to draw on the screen.
	int x_offset = terrain_pos - terrain_width;

	for (unsigned int x=0; x < terrain_width; x++)
	{
		for (unsigned int y=0; y < terrain_height; y++)
		{
			if (int(y) < percent_to_pixels(terrain[x+x_offset].sct, terrain_height))
				mvaddch(y, x, SOLID_SYMBOL);
			else if (y >= terrain_height-percent_to_pixels(terrain[x+x_offset].sgm, terrain_height))
				mvaddch(y, x, SOLID_SYMBOL);
			else
				mvaddch(y, x, EMPTINESS_SYMBOL);
		}
	}
}

void draw_shields(const vector <Step> terrain, const int terrain_pos, const unsigned int terrain_height, const unsigned int terrain_width)
{
	// terrain_pos is the pointer to the LAST (right-most) element to draw on the screen.
	int x_offset = terrain_pos - terrain_width;

	for (unsigned int x=0; x < terrain_width; x++)
	{
		for (unsigned int y=0; y < terrain_height; y++)
		{
			if (terrain[x+x_offset].shield_y != -1)
			{
				attron(A_BLINK);
				mvaddch(percent_to_pixels(100-terrain[x+x_offset].shield_y, terrain_height), x, SHIELD_SYMBOL);
				attroff(A_BLINK);
			}
		}
	}
}

void draw_trails(const unsigned int terrain_width, const Copter &copter, const Trail &trail)
{
	for (unsigned int x=0; x < terrain_width; x++)
	{
		int trail_element = copter.query_x()-x-1;
		if (trail_element >= 0 && trail_element < trail.size())
		{
			int trail_pos = trail.query_fromback(trail_element).pos;
			bool trail_type = trail.query_fromback(trail_element).type;
			if (trail_type)
				mvaddch(trail_pos, x, ASCENT_SYMBOL);
		}
	}
}

void draw_copter(const Copter &copter)
{
	attron(A_REVERSE);
	mvaddch(copter.query_y(), copter.query_x(), HELICOPTER_SYMBOL);
	attroff(A_REVERSE);
}

void draw_statusbar(const string message, const int y_pos, const int distance)
{
	string distance_msg;
	string status;
	distance_msg = "Distance traveled: "+int_string(distance);
	status = spread(message, distance_msg, COLS);
	mvaddstr(y_pos-1, 0, status.c_str());
}

void plunge_copter(Copter &copter, const unsigned int terrain_height, const unsigned int sgm_pixels, const unsigned int distance, const TerrainMetadata t_metadata)
{
	draw_statusbar(DEATH_STATUS_MSG, LINES, distance);
	render_screen();
	beep();
	flash();

	/* Physics equation for displacement in free-fall based on time is:
		x = v_i*t + 1/2 a*t^2
	*/
	unsigned int fall_counter=0;
	while (copter.query_y() < int(terrain_height-sgm_pixels-1))
	{
		usleep(t_metadata.step_delay*1000);
		fall_counter++;
		copter.incr_y(double_to_int(pow(double(fall_counter), 2)*t_metadata.gravity/Y_MOTION_DAMPER));
		if (copter.query_y() > int(terrain_height-sgm_pixels-1))
			copter.set_y(terrain_height-sgm_pixels-1);
		draw_copter(copter);
		mvaddch(copter.query_y()-1, copter.query_x(), '|'); // the freefall wind
		render_screen();
	}
	// exploding
	mvaddch(copter.query_y()-1, copter.query_x()-1, '\\');
	mvaddch(copter.query_y()-1, copter.query_x()+1, '/');
	mvaddch(copter.query_y(), copter.query_x()-1, '-');
	mvaddch(copter.query_y(), copter.query_x()+1, '-');
	render_screen();
	beep();
	flash();
	sleep(5);
}
