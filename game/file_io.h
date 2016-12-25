// License: GNU General Public License v2

#ifndef _FILE_IO_H_
#define _FILE_IO_H_

#include <vector>
#include <fstream>

#include "trail.h"
#include "terrain.h"

// Preconditions: the file referenced by infile exists and is open for reading
// Postconditions: the terrain structure array and the metadata structure are filled
// Return values: 0 if terrain file was parsed successfully; >0 otherwise
int proc_terrain(std::ifstream &, vector <Step> &, TerrainMetadata &, int &);

// Preconditions: none
// Purpose: makes sure that the terrain file becomes closed no matter what.
// Return values: 1 if a file named terrain_file does not exist; else whatever proc_terrain() returns
int proc_terrain_wrapper(const string, vector <Step> &, TerrainMetadata &, int &);

// Preconditions: none
// Postconditions: terrain structure array and metadata structure are filled
// Purpose: interprets error codes returned by lower-level file-handling functions and prints them to stdout
// Return values: true if no problems; false otherwise
bool fetch_terrain(vector <Step> &, const string, TerrainMetadata &);

#endif
