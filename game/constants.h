#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#include <string>
#include <curses.h>

const std::string PROGRAM_VERSION = "0.1";
const std::string PROGRAM_RELDATE = "2004-07-19";

const int INPUT_DELAY = 2;
const int SHIELD_DURATION_MIN = 5;
const int SHIELD_DURATION_MAX = 100;
const int TERRAIN_STEPDELAY_MIN = 150;
const int TERRAIN_STEPDELAY_MAX = 2000;
const double TERRAIN_GRAVITY_MIN = 0;
const double TERRAIN_GRAVITY_MAX = 10;
const double HELICOPTER_ACCELERATION = -10; // how powerful the copter's engine is; negative due to ncurses coordinate system
const double Y_MOTION_DAMPER = 8;
const std::string SUCCESS_END_MSG = "You have guided your trusty copter to the end of this terrain.";
const std::string DEATH_END_MSG = "Your helicopter crashed!";
const std::string DEATH_STATUS_MSG = "You crash, and your copter plunges to its demise!";
const std::string PAUSE_STATUS_MSG = "-- PAUSED --";
const char HELICOPTER_SYMBOL = 'O';
const char SHIELD_SYMBOL = '@';
const char SOLID_SYMBOL = '#';
const char ASCENT_SYMBOL = '*';
const char EMPTINESS_SYMBOL = ' ';
const int ASCENT_KEY = KEY_UP;

const int VIEWER_SCROLL_DELTA = 2;

#endif
