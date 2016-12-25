// License: GNU General Public License v2

// This is a program that allows one to view the terrain without worries about the helicopter.
// It is useful for terrain development.

#include <curses.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "constants.h"
#include "generic.h"
#include "terrain.h"
#include "file_io.h"
#include "curses.h"

int main(int argc, char *argv[])
{
	string terrain_file;
	vector <Step> terrain;
	TerrainMetadata t_metadata;

	if (argc > 2)
	{
		cerr << "This program accepts at most one parameter -- the terrain filename." << endl;
		return 1;
	}
	else if (argc == 2)
		terrain_file = string(argv[1]);
	else
	{
		terrain_file = trim_whitespace(readline("Enter filename of desired terrain: "));
	}
	if (!fetch_terrain(terrain, terrain_file, t_metadata))
		return 2;

	cout << "You may press F1 at any point to pause." << endl;
	init_curses(4);
	for (unsigned int i=COLS; i < terrain.size(); i += VIEWER_SCROLL_DELTA)
	{
		draw_background(terrain, i, LINES-1, COLS);
		draw_shields(terrain, i, LINES-1, COLS);
		draw_statusbar(int_string(double_to_int(i*100.0/terrain.size()))+"% completed", LINES, i);
		render_screen();
		if (getch() == KEY_F(1))
		{
			draw_statusbar(PAUSE_STATUS_MSG, LINES, i);
			while (getch() != KEY_F(1));
		}
		usleep(t_metadata.step_delay*1000);
	}
	endwin();
	cout << "Finished." << endl;
	return 0;
}
