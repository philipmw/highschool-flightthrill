// License: GNU General Public License v2

// This is a program that allows one to convert a terrain data file to an ASCII representation thereof.
// It is useful for terrain development and sharing.

#include <curses.h> //required by GNU readline
#include <readline/readline.h>
#include <readline/history.h>

#include "constants.h"
#include "generic.h"
#include "terrain.h"
#include "file_io.h"
#include "rel_coords.h"

int main(int argc, char *argv[])
{
	string terrain_file, output_file;
	vector <Step> terrain;
	TerrainMetadata t_metadata;
	ofstream outfile;

	if (argc > 3)
	{
		cerr << "This program accepts at most two parameters -- the terrain filename followed by the output filename." << endl;
		return 1;
	}
	else if (argc == 2)
		terrain_file = string(argv[1]);
	else if (argc == 3)
	{
		terrain_file = string(argv[1]);
		output_file = string(argv[2]);
	}
	else // argc == 1
	{
		terrain_file = trim_whitespace(readline("Enter filename of desired terrain: "));
		output_file = trim_whitespace(readline("Enter output filename: "));
	}

	if (output_file.length() < 1)
	{
		cerr << "The output filename cannot be blank." << endl;
		return 2;
	}
	outfile.open(output_file.c_str());
	if (!fetch_terrain(terrain, terrain_file, t_metadata))
		return 3;

	// Rendering row-by-row
	const unsigned int SCREEN_LENGTH = 24;
	for (unsigned int row=0; row < SCREEN_LENGTH; row++)
	{
		for (unsigned int col=0; col < terrain.size(); col++)
		{
			if (percent_to_pixels(terrain[col].sct, SCREEN_LENGTH) >= int(row))
				outfile << SOLID_SYMBOL;
			else if (percent_to_pixels(terrain[col].sgm, SCREEN_LENGTH) >= int(SCREEN_LENGTH-row))
				outfile << SOLID_SYMBOL;
			else if (percent_to_pixels(terrain[col].shield_y, SCREEN_LENGTH) == int(row))
				outfile << SHIELD_SYMBOL;
			else
				outfile << EMPTINESS_SYMBOL;
		}
		outfile << endl;
	}
	outfile.close();
	cout << "Finished." << endl;
	return 0;
}
