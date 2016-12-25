// License: GNU General Public License v2
// Functions for file input/output, shared among multiple programs.

#include "constants.h"
#include "generic.h"
#include "file_io.h"

// Preconditions: none
// Postconditions: all items in the step structure are initialized
void initialize_step(Step &step)
{
	step.sct = step.sgm = step.shield_y = -1;
}

int proc_terrain(ifstream &infile, vector <Step> &terrain, TerrainMetadata &metadata, int &problematic_line)
{
	string tempinput, sct_str, sgm_str;

	// starting with metadata
	problematic_line = 1;
	getline(infile, tempinput);
	tempinput = trim_whitespace(tempinput);
	if (tempinput.find(TERRAIN_AUTHOR_STR) == 0)
		metadata.author = trim_whitespace(tempinput.substr(TERRAIN_AUTHOR_STR.length()));
	else
		return 2;
	problematic_line++;
	getline(infile, tempinput);
	tempinput = trim_whitespace(tempinput);
	if (tempinput.find(TERRAIN_TITLE_STR) == 0)
		metadata.title = trim_whitespace(tempinput.substr(TERRAIN_TITLE_STR.length()));
	else
		return 3;
	problematic_line++;
	getline(infile, tempinput);
	tempinput = trim_whitespace(tempinput);
	if (tempinput.find(TERRAIN_SUCCESS_STR) == 0)
		metadata.success_msg = trim_whitespace(tempinput.substr(TERRAIN_SUCCESS_STR.length()));
	else
		return 4;
	problematic_line++;
	getline(infile, tempinput);
	tempinput = trim_whitespace(tempinput);
	if (tempinput.find(TERRAIN_DEATH_STR) == 0)
		metadata.death_msg = trim_whitespace(tempinput.substr(TERRAIN_DEATH_STR.length()));
	else
		return 5;
	problematic_line++;
	getline(infile, tempinput);
	tempinput = trim_whitespace(tempinput);
	if (tempinput.find(TERRAIN_COPTER_X_STR) == 0)
	{
		tempinput = tempinput.substr(TERRAIN_COPTER_X_STR.length());
		// now just the coordinate is present as a string
		if (tempinput.length() < 1) // empty value
			return 6;
		if (string_int(tempinput.c_str(), tempinput.length(), metadata.copter_initx_perc)) // couldn't convert X to integer
			return 7;
		if (metadata.copter_initx_perc < 0 || metadata.copter_initx_perc >= 100) // out of bounds
			return 8;
	}
	else
		return 6;
	problematic_line++;
	getline(infile, tempinput);
	tempinput = trim_whitespace(tempinput);
	if (tempinput.find(TERRAIN_STEPDELAY_STR) == 0)
	{
		tempinput = tempinput.substr(TERRAIN_STEPDELAY_STR.length());
		// now just the step delay is present as a string
		if (tempinput.length() < 1) // empty value
			return 10;
		if (string_int(tempinput.c_str(), tempinput.length(), metadata.step_delay)) // couldn't convert delay string to integer
			return 11;
		if (metadata.step_delay < TERRAIN_STEPDELAY_MIN || metadata.step_delay > TERRAIN_STEPDELAY_MAX)
			return 12;
	}
	else
		return 9;
	problematic_line++;
	getline(infile, tempinput);
	tempinput = trim_whitespace(tempinput);
	if (tempinput.find(TERRAIN_GRAVITY_STR) == 0)
	{
		tempinput = tempinput.substr(TERRAIN_GRAVITY_STR.length());
		// now just the acceleration is present as a string
		if (tempinput.length() < 1) // empty value
			return 14;
		if (string_double(tempinput.c_str(), tempinput.length(), metadata.gravity)) // couldn't convert gravity string to double
			return 15;
		if (metadata.gravity < TERRAIN_GRAVITY_MIN || metadata.gravity > TERRAIN_GRAVITY_MAX)
			return 16;
	}
	else
		return 13;
	// finished reading metadata; now reading terrain.
	// error return codes have a gap between them for future metadata expansion.

	while (!infile.eof())
	{
		getline(infile, tempinput);
		tempinput = trim_whitespace(tempinput);
		problematic_line++;
		if (tempinput.length() == 0) // blank line
			continue;
		if (tempinput[0] == '#') // comment
			continue;
		if (tempinput.find(SHIELD_STR) == 0) // this is a shield declaration
		{
			tempinput = tempinput.substr(SHIELD_STR.length(), tempinput.length());
			if (terrain.size() == 0)
				return 20;
			if (tempinput.find(' ') == string::npos)
				return 21;
			string shield_y_str, shield_duration_str;
			shield_y_str = tempinput.substr(0, tempinput.find(' '));
			shield_duration_str = tempinput.substr(tempinput.find(' ')+1, tempinput.length());
			if (string_int(shield_y_str.c_str(), shield_y_str.length(), terrain[terrain.size()-1].shield_y))
				return 22;
			if (string_int(shield_duration_str.c_str(), shield_duration_str.length(), terrain[terrain.size()-1].shield_duration))
				return 23;
			if (terrain[terrain.size()-1].shield_y < 0 || terrain[terrain.size()-1].shield_y > 100)
				return 24;
			if (terrain[terrain.size()-1].shield_duration < SHIELD_DURATION_MIN || terrain[terrain.size()-1].shield_duration > SHIELD_DURATION_MAX)
				return 25;
			// Keep in mind that shield_y is looking from the bottom, while sct is looking from the top
			if (terrain[terrain.size()-1].shield_y >= 100-terrain[terrain.size()-1].sct)
				return 26;
			if (terrain[terrain.size()-1].shield_y <= terrain[terrain.size()-1].sgm)
				return 27;
			//cout << "shield now at index " << terrain.size()-1 << " with value " << terrain[terrain.size()-1].shield_y << ", duration " << terrain[terrain.size()-1].shield_duration << endl;
		}
		else // this is not a shield declaration -- must be a regular terrain step
		{
			// return error codes have a gap here for future shield checking expansion
			if (tempinput.find(' ') == string::npos) // no spaces
				return 40;
			sct_str = tempinput.substr(0, tempinput.find(' '));
			sgm_str = tempinput.substr(tempinput.find(' ')+1, tempinput.length());
			if (sct_str.length() < 1 || sct_str.length() > 2)
				return 41;
			if (sgm_str.length() < 1 || sgm_str.length() > 2)
				return 42;
			terrain.resize(terrain.size()+1);
			initialize_step(terrain[terrain.size()-1]);
			if (string_int(sct_str.c_str(), sct_str.length(), terrain[terrain.size()-1].sct))
				return 43;
			if (string_int(sgm_str.c_str(), sgm_str.length(), terrain[terrain.size()-1].sgm))
				return 44;
			if (terrain[terrain.size()-1].sct < 0 || terrain[terrain.size()-1].sgm < 0)
				return 45;
			if (terrain[terrain.size()-1].sct >= 100 || terrain[terrain.size()-1].sgm >= 100)
				return 46;
			if (terrain[terrain.size()-1].sct + terrain[terrain.size()-1].sgm >= 100)
				return 47;
		}
	}
	terrain.resize(terrain.size()-1);
	return 0;
}

int proc_terrain_wrapper(const string terrain_file, vector <Step> &terrain, TerrainMetadata &metadata, int &problematic_line)
{
	ifstream infile;
	infile.open(terrain_file.c_str());
	if (infile.fail())
		return 1;
	int return_code = proc_terrain(infile, terrain, metadata, problematic_line);
	infile.close();
	return return_code;
}

bool fetch_terrain(vector <Step> &terrain, const string terrain_file, TerrainMetadata &metadata)
{
	bool good_terrain = false;
	int problematic_line;

	cout << "Processing the terrain file... ";
	int terrain_value = proc_terrain_wrapper(terrain_file, terrain, metadata, problematic_line);
	if (terrain_value == 0)
	{
		cout << "success." << endl;
		cout << "This terrain is " << terrain.size() << " steps long." << endl;
		good_terrain = true;
	}
	else
	{
		if (terrain_value != 1)
			cout << "failure on line " << problematic_line << "." << endl;
		else
			cout << endl;
		cout << "Problem: ";
		switch (terrain_value)
		{
			case 1: cout << "the file \"" << terrain_file << "\" does not exist." << endl;
				break;
			case 2: cout << "the Author line is missing." << endl;
				break;
			case 3: cout << "the Title line is missing." << endl;
				break;
			case 4: cout << "the success (terrain completion) message is missing." << endl;
				break;
			case 5: cout << "the death message is missing." << endl;
				break;
			case 6: cout << "the copter's initial X coordinate is missing." << endl;
				break;
			case 7: cout << "the copter's initial X coordinate is not an integer." << endl;
				break;
			case 8: cout << "the copter's initial X coordinate is not a valid percentage (0-100)" << endl;
				break;
			case 9: cout << "the terrain's step delay line is missing." << endl;
				break;
			case 10: cout << "the terrain's step delay value is missing." << endl;
				break;
			case 11: cout << "the terrain's step delay is not an integer." << endl;
				break;
			case 12: cout << "the terrain's step delay must be from " << TERRAIN_STEPDELAY_MIN << " to " << TERRAIN_STEPDELAY_MAX << "." << endl;
				break;
			case 13: cout << "the terrain's gravity line is missing." << endl;
				break;
			case 14: cout << "the terrain's gravity value is missing." << endl;
				break;
			case 15: cout << "the terrain's gravity is not a double." << endl;
				break;
			case 16: cout << "the terrain's gravity must be from." << TERRAIN_GRAVITY_MIN << " to " << TERRAIN_GRAVITY_MAX << "." << endl;
				break;

			case 20: cout << "a shield declaration must FOLLOW that of stalactites/stalagmites." << endl;
				break;
			case 21: cout << "a shield is defined by two integers: Y coordinate and duration." << endl;
				break;
			case 22: cout << "a shield value must be an integer." << endl;
				break;
			case 23: cout << "a shield's duration must be an integer." << endl;
				break;
			case 24: cout << "a shield's Y coordinate must be a percentage from 0 to 100." << endl;
				break;
			case 25: cout << "a shield's duration must be from " << SHIELD_DURATION_MIN << " to " << SHIELD_DURATION_MAX << "." << endl;
				break;
			case 26: cout << "the shield is embedded into the stalactite." << endl;
				break;
			case 27: cout << "the shield is embedded into the stalagmite." << endl;
				break;

			case 40: cout << "each step must have a Y value for both stalactites and stalagmites." << endl;
				break;
			case 41: cout << "a stalactite must be a one- or two-digit integer." << endl;
				break;
			case 42: cout << "a stalagmite must be a one- or two-digit integer." << endl;
				break;
			case 43: cout << "unable to convert a supposed stalactite integer in the file to a real integer." << endl;
				break;
			case 44: cout << "unable to convert a supposed stalagmite integer in the file to a real integer." << endl;
				break;
			case 45: cout << "every integer must be positive." << endl;
				break;
			case 46: cout << "every integer must be under 100." << endl;
				break;
			case 47: cout << "the sum of a stalactite and a stalagmite pair must be under 100." << endl;
			default: cout << "undefined error!" << endl;
		}
	}

	if (!good_terrain)
		return 0;
	return 1;
}
