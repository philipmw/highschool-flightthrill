// License: GNU General Public License v2
// Last modified: 2004-01-20
#include "generic.h"

// Function programmed mostly by Deborah Nasir
// Purpose: Converts an integer to a string and returns the string.
string int_string(int num)
{
	const int PLACES = 10;
	string temp;
	char stack[PLACES];		//initialize the stack
	int top = PLACES-1;		//initialize the top of stack
	bool is_negative=false;

	if (num < 0)
	{
		is_negative = true;
		num *= -1;
	}
	for (int i=0; i < PLACES; i++)
		stack[i] = '0';
	do
	{
		stack[top] = char (int ('0') + (num % 10));

		top--;
		num /= 10;
	}
	while (num > 0);
	top = 0;		//reinitialze top
	while (top < PLACES-1 && stack[top] == '0')	//filter out leading zeros
		top++;
	while (top < PLACES)		//put the digits in the string
	{
		temp += stack[top];
		top++;
	}
	if (is_negative)
		temp = "-"+temp;
	return temp;
}

int string_int(const char charString[], const int length, unsigned int &output)
{
	int temp_output, retval;
	retval = string_int(charString, length, temp_output);
	output = unsigned(temp_output);
	return retval;
}

int string_int(const char charString[], const int length, int &output)
{
	const int MAX_DIGITS = 7;
	int intArray[MAX_DIGITS] = { 0 }, final_integer = 0;
	bool negative = 0;

	if (length < 1 || length > MAX_DIGITS)
		return 1;
	if (charString[0] == '-')
		negative = true;

	// Checking for non-numeric elements.
	for (int a = negative; a < length; a++)
		if (charString[a] < '0' || charString[a] > '9')
			return 2;

	// Before anything, the array contains something like {'3', '1', '7'}.

	for (int i = negative; i < length; i++)
		intArray[i] = int (charString[i]) - '0';

	// Now the intArray contains something like {3, 1, 7}.

	for (int j = negative; j < length; j++)
		intArray[j] *= int (pow(10.0, (length - 1 - j)));

	// Now the intArray contains something like {300, 10, 7}.

	for (int k = negative; k < length; k++)
		final_integer += intArray[k];

	if (negative)
		final_integer *= -1;

	output = final_integer;
	return 0;
}

int string_double(const char charString[], const int length, double &output)
{
	const int MAX_DIGITS = 8;
	char charArray[MAX_DIGITS];
	int counter=0, temp_int=0;
	double final_double;

	if (length < 1 || length > MAX_DIGITS)
		return 1;

	while (charString[counter] != '.' && counter < length)
	{
		charArray[counter] = charString[counter];
		counter++;
	}
	// now we have the [negative?] integer part of the double in integer[].
	// time to convert that string into an integer (for now).
	if (counter != 0 && string_int(charArray, counter, temp_int))
	{
		// unsuccessfully converted the first part.  Poop.
//		cout << "Unsuccessfully converted the integer part of the double." << endl;
		return 2;
	}
	final_double = double(temp_int); // depositing the first part.
	// Time to deal with the fractional part.
	if (counter >= length-1) // Is there a fractional part at all?
	{
		// no fractional part.  We're done.
		output = final_double;
		return 0;
	} else {
		counter++; // getting past the dot.
		for (int i=counter; i < length; i++)
			charArray[i-counter] = charString[i];
		if (string_int(charArray, length-counter, temp_int))
		{
//			cout << "Unsuccessfully converted the decimal part of the double." << endl;
			return 4; // the second part proved invalid.
		}
		if (charString[0] == '-') // if it's a negative number...
			temp_int *= -1;
		final_double += double(temp_int) * pow(10.0, counter-length);
	}
	output = final_double;
	return 0;
}

int alert(const string msg, const bool query)
{
	const int DELAY = 5;
	static int alerts = 0;

	if (query)
	{
		return alerts;
	}
	else
	{
		alerts++;
		cerr << '\a' << msg << endl << endl;
		#ifdef __MSDOS__
		delay(DELAY*1000);
		#else
		sleep(DELAY);
		#endif
	}
	return 0;
}

int move_file(const string src, const string dst)
{
	string temp;
	int num_lines = 0,
	 line_count = 1;
	ofstream destination;
	ifstream source;

	source.open(src.c_str());
	if (src == dst)
	{
		source.close();
		destination.close();
		alert("Cannot move data: source filename may not equal destination filename.", 0);
		return -1;
	}
	if (source.fail())
	{
		source.close();
		alert("The source file ("+src+") could not be opened for copying data.", 0);
		return -1;
	}
	remove(dst.c_str());
	// Getting the number of lines in source file
	while (!source.eof())
	{
		getline(source, temp);
		num_lines++;
	}
	source.clear();
	source.seekg(0);
	/* ***************** SAMPLE CODE *******************
	get length of file:
	is.seekg (0, ios::end);
	length = is.tellg();
	is.seekg (0, ios::beg);
	***************************************************/

	//cout << "Moving data from " << src << " to " << dst << "..." << endl;
	destination.open(dst.c_str());
	while (line_count <= num_lines)
	{
		getline(source, temp);
		//cout << "Writing: \"" << temp << "\"\n";
		destination << temp;
		if (line_count != num_lines)
			destination << endl;
		line_count++;
	}

	source.close();
	destination.close();

	remove(src.c_str());	// deleting the old file.
	return 1;
}

string spread(const string str1, const string str2, const unsigned int line_len)
{
	string result;

	result = str1;
	for (unsigned int i = 1; i <= line_len - (str1.length() + str2.length()); i++)
		result += " ";
	result += str2;
	return result;
}

int double_to_int(const double num)
{
	int rounded;
	if (num >= 0)
		rounded = int(num+0.5);
	else
		rounded = int(num-0.5);
	return rounded;
}

string trim_whitespace(string str)
{
	const int WHITESPACE_SIZE = 3;
	const char whitespace[WHITESPACE_SIZE] = { ' ', '\t', '\n' };

	int counter=0;
	bool chop_off;
	do
	{
		chop_off = false;
		for (int i=0; i < WHITESPACE_SIZE; i++)
		{
			if (str[counter] == whitespace[i])
				chop_off = true;
		}
		if (chop_off)
			str = str.substr(1, str.length()-1);
		counter++;
	} while (chop_off);
	// the first part is chopped off.
	do
	{
		chop_off = false;
		for (int j=0; j < WHITESPACE_SIZE; j++)
		{
			if (str[str.length()-1] == whitespace[j])
				chop_off = true;
		}
		if (chop_off)
		{
			str = str.substr(0, str.length()-1);
		}
	} while (chop_off);
	// done
	return str;
}
