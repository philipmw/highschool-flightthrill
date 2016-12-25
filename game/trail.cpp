// License: GNU General Public License v2

#include <iostream>
#include "trail.h"

Trail::Trail()
{
	array.resize(0);
}

void Trail::append(int pos, int type)
{
	array.resize(array.size()+1);
	array[array.size()-1].pos = pos;
	array[array.size()-1].type = type;
}

// precondition: 0 <= distance < array.length()
TrailBit Trail::query_fromback(int distance) const
{
	int element = array.size()-1-distance;
	if (element < 0)
		element = 0;
	return array[element];
}

void Trail::output_last(int quantity)
{
	unsigned int i = array.size()-quantity;
	if (i < 0)
		i = 0;
	for (; i < array.size(); i++)
	{
		std::cout << array[i].pos << ',' << array[i].type << ' ';
	}
}

int Trail::size() const
{
	return array.size();
}

int Trail::num_up() const
{
	int counter=0;
	for (unsigned int i=0; i < array.size(); i++)
		if (array[i].type)
			counter++;
	return counter;
}
