// License: GNU General Public License v2

#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include <string>

struct Step {
	int sct; // stalactite percentage
	int sgm; // stalagmite percentage
	int shield_y; // shield Y coordinate
	int shield_duration; // shield lifetime
};

struct TerrainMetadata {
	std::string author;
	std::string title;
	std::string success_msg;
	std::string death_msg;
	int copter_initx_perc;
	int step_delay;
	double gravity;
};

const std::string TERRAIN_AUTHOR_STR = "Author: ";
const std::string TERRAIN_TITLE_STR = "Title: ";
const std::string TERRAIN_SUCCESS_STR = "Success message: ";
const std::string TERRAIN_DEATH_STR = "Death message: ";
const std::string TERRAIN_COPTER_X_STR = "Copter start: ";
const std::string TERRAIN_STEPDELAY_STR = "Step delay: ";
const std::string TERRAIN_GRAVITY_STR = "Gravity: ";

const std::string SHIELD_STR = "shield: ";

#endif
