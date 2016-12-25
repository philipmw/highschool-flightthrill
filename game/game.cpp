// Programmer: Philip M. White
// Computer Science 3 AP final project, year 2003
// School: Pasadena High, Pasadena Independent School District
// Instructor: Deborah Nasir
// Released under the GNU General Public License v2 in July 2004

#include <curses.h>
#include <stdlib.h>
#include "generic.h"
#include <readline/readline.h>
#include <readline/history.h>

#include "file_io.h"
#include "constants.h"
#include "copter.h"
#include "terrain.h"
#include "trail.h"
#include "curses.h"
#include "rel_coords.h"

struct GameResults {
	int status;
	int distance;
	int fuel_used;
	int duration;
	int completion;
};

// Preconditions: none
// Postconditions: none
// Purpose: to print a terrain's metadata to stdout
void print_terrain_metadata(const TerrainMetadata metadata)
{
	cout << "The loaded terrain's information:" << endl;
	cout << "   Author:\t\t" << metadata.author << endl;
	cout << "   Title:\t\t" << metadata.title << endl;
	cout << "   Success message:\t" << metadata.success_msg << endl;
	cout << "   Death message:\t\t" << metadata.death_msg << endl;
	cout << "   Copter initial X:\t" << metadata.copter_initx_perc << endl;
}

// Preconditions: terrain structure array and t_metadata structure are properly filled
// Postconditions: GameResults structure contains the outcome of the game
GameResults play_game(vector <Step> &terrain, const TerrainMetadata &t_metadata)
{
	Trail trail;
	GameResults results;
	string status_message;
	time_t start_time, end_time;

	unsigned int terrain_index, terrain_limit; // in pixels (updated once per main loop cycle)
	unsigned int terrain_height, terrain_width; // in pixels (updated once per game)
	unsigned int sct_pixels, sgm_pixels; // how many pixels from zero each blockade occupies at copter's screen X coordinate (updated once per main loop cycle)
	int life_status;

	results.status = 0;
	results.distance = 0;
	results.fuel_used = 0;
	init_curses(INPUT_DELAY); // WE ARE NOW IN NCURSES
	if (has_key(ASCENT_KEY) == ERR)
	{
		endwin();
		cerr << "Your keyboard/terminal do not support the Up arrow key." << endl;
		results.status = 2;
	}
	if (terrain.size() < (unsigned int)COLS)
	{
		endwin();
		cerr << "Your terrain is too short to even fill the terminal screen.\nPlease lengthen it or narrow your terminal emulator before playing." << endl;
		results.status = 3;
	}

	if (results.status != 0)
		return results;

	// Everything is good enough to begin.
	terrain_height = LINES-1;
	terrain_width = COLS;
	terrain_limit = terrain_width;
	Copter copter(
			percent_to_pixels(t_metadata.copter_initx_perc, terrain_width),
			terrain_height - percent_to_pixels(terrain[percent_to_pixels(t_metadata.copter_initx_perc, terrain_width)].sgm, terrain_height)-1,
			0, 0, terrain_height, terrain_width
			);

	draw_background(terrain, terrain_limit, terrain_height, terrain_width);
	draw_shields(terrain, terrain_limit, terrain_height, terrain_width);
	draw_copter(copter);
	draw_statusbar("Ready for take-off.  (Terrain dimensions: "+int_string(terrain_width)+"x"+int_string(terrain_height)+")", LINES, results.distance);
	render_screen();
	while (get_character() != ASCENT_KEY); //stop here until Up key is pressed
	// taking off!
	
        time(&start_time);
	do // main loop
	{
		int keypress = get_character();
		bool is_going_up = (keypress == ASCENT_KEY);
		if (keypress == KEY_F(1)) // user wants to pause?
		{
			draw_statusbar(PAUSE_STATUS_MSG, LINES, results.distance);
			while (get_character() != KEY_F(1)); // stop here until user unpauses
		}

		if (terrain_limit < terrain.size()-1) // not at the end of the terrain
		{
			// advancing the copter
			terrain_limit++;
			copter.affect_prot(-1);
		}
		else
			copter.incr_x(1);
		terrain_index = terrain_limit-terrain_width+copter.query_x()-1;
		results.distance++;
		trail.append(copter.query_y(), is_going_up); // smoke puff
		copter.apply_physics(is_going_up, terrain_height, t_metadata.gravity);
		results.completion = double_to_int(double(terrain_index+1)*100/terrain.size());
		// normally, all copter motion has been calculated by now.

		draw_background(terrain, terrain_limit, terrain_height, terrain_width);
		life_status = query_life_status(copter, terrain_height, terrain_width); //samples the background

		if (terrain_index < terrain.size())
		{
			sct_pixels = percent_to_pixels(terrain[terrain_index].sct, terrain_height);
			sgm_pixels = percent_to_pixels(terrain[terrain_index].sgm, terrain_height);
		}
		if ((life_status == 1 || life_status == 2 || life_status == 3) && copter.query_prot() > 0) // protection triggered
		{
			copter.reset_physics();
			life_status = 0; // faking that it's Saul-good
			copter.set_y(sct_pixels + double_to_int(double(terrain_height-(sct_pixels+sgm_pixels))/2));
			copter.reset_prot();
			beep();
		}
		if (copter.query_y() < int(sct_pixels))
			copter.set_y(sct_pixels-1);
		if (copter.query_y() > int(terrain_height-sgm_pixels))
			copter.set_y(terrain_height-sgm_pixels);
		if (copter.query_y() >= int(terrain_height))
			copter.set_y(terrain_height-1);
		draw_copter(copter);
		draw_trails(terrain_width, copter, trail);
		draw_shields(terrain, terrain_limit, terrain_height, terrain_width);

		if (copter.query_prot() > 0)
			status_message = "Shields active ("+int_string(copter.query_prot())+")";
		else if (results.distance < 50)
			status_message = t_metadata.author + " - " + t_metadata.title;
		else
			status_message = int_string(results.completion)+"% complete";
		draw_statusbar(status_message, LINES, results.distance);

		if (copter.query_y() == int(terrain_height)-percent_to_pixels(terrain[terrain_index].shield_y, terrain_height)) //landed on a shield
		{
			copter.affect_prot(terrain[terrain_index].shield_duration);
			terrain[terrain_index].shield_y = -1;
		}
		render_screen();
		usleep(t_metadata.step_delay*1000);
	} while (life_status == 0); // repeat main loop while all's peachy keen
        time(&end_time);
	/*
	draw_statusbar("You died with errorcode "+int_string(life_status), LINES, results.distance);
	render_screen();
	sleep(2);
	*/
	if (life_status != 5)
		plunge_copter(copter, terrain_height, sgm_pixels, results.distance, t_metadata);
	// emptying the input buffer before exiting
	while (get_character() != ERR);
	endwin(); // ncurses is over; back to console mode
	cout << endl;
	if (copter.query_x() >= terrain_width) // terrain is over
	{
		if (t_metadata.success_msg != "none")
			cout << t_metadata.success_msg << endl << endl;
		else
			cout << SUCCESS_END_MSG << endl;
	}
	else if (copter.query_x() < terrain_width)
	{
		if (t_metadata.death_msg != "none")
			cout << t_metadata.death_msg << endl << endl;
		else
			cout << DEATH_END_MSG << endl;
	}
	results.fuel_used = trail.num_up();
        results.duration = int(difftime(end_time, start_time));

	return results;
}

int main(int argc, char *argv[])
{
	GameResults results;
	string terrain_file;
	vector <Step> terrain; // in screen percentages
	TerrainMetadata t_metadata;

	cout << "************************************" << endl;
	cout << "     Welcome to FlightThrill !" << endl;
	cout << "version " << PROGRAM_VERSION << ", release date " << PROGRAM_RELDATE << endl;
	cout << "************************************" << endl;
	cout << "compiled on " << DATESTAMP << endl << endl;
	cout << "Written by Philip M. White." << endl;
	cout << "Dedicated to Deborah Nasir." << endl;
	cout << "Licensed under the GNU General Public License, version 2 or later." << endl << endl;
	cout << "You may press F1 at any time during the game to pause it or ^C to end it." << endl << endl;
	if (argc > 2)
	{
		cout << "This program accepts at most one parameter -- the terrain filename." << endl;
		return 1;
	}
	else if (argc == 2)
		terrain_file = string(argv[1]);
	else
	{
		terrain_file = trim_whitespace(readline("Enter filename of desired terrain: "));
		//cout << "Enter filename of desired terrain: ";
		//getline(cin, terrain_file);
	}
	if (!fetch_terrain(terrain, terrain_file, t_metadata))
		return 1;
	//print_terrain_metadata(t_metadata);

	cout << "The game will begin in 3 seconds.  Get psyched!!!" << endl;
	sleep(3);
	results = play_game(terrain, t_metadata); // the big kahuna

	if (results.status != 0)
		return results.status;
	cout << "Game duration:\t\t" << double_to_int(results.duration/60.0) << ":" << (results.duration%60<10 ? "0" : "") << results.duration%60 << endl;
	cout << "Fuel used:\t\t" << results.fuel_used << " units" << endl;
	cout << "Distance traveled:\t" << results.distance << endl;
	cout << "Terrain covered:\t" << results.completion << "%" << endl;
	double composite_score = 1.5*results.distance-results.fuel_used;
	if (composite_score < 0)
		composite_score = 0;
	cout << "Composite score:\t" << composite_score << endl;

	return 0;
}
