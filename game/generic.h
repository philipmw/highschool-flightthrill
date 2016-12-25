// License: GNU General Public License v2

#ifndef _GENERIC_H_
#define _GENERIC_H_

#include <iostream>
#include <string>
using namespace std;
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

// Converts an integer to an string. (written by D. Nasir)
string int_string(int);
// Converts a string into an integer and saves to the last parameter.
// 0 upon success
int string_int(const char [], const int, int &);
int string_int(const char [], const int, unsigned int &);
// Converts a string into a double and saves to the last parameter.  Returns true upon success.
int string_double(const char [], const int, double &);
// Alerts the operator with a message and keeps track of errors
int alert(const string, const bool);
// Moves a file from source to destination; returns -1 if unsuccessful.
int move_file(const string, const string);
// Print two strings on the same line, spaced out over the entire line.
string spread(const string, const string, const unsigned int);
// Rounds doubles properly (up if >= .5, else down) and converts them to an integer
int double_to_int(const double);
// Cut off various whitespaces from start and end of the string, then return it
string trim_whitespace(string);

#endif
