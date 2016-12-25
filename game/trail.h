// License: GNU General Public License v2

#ifndef _TRAIL_H_
#define _TRAIL_H_

#include <vector>

struct TrailBit {
	int pos;
	bool type;
};

class Trail {
	public:
		Trail();
		void append(int, int);
		TrailBit query_fromback(int) const;
		void output_last(int);
		int size() const;
		int num_up() const;
		
	private:
		std::vector <TrailBit> array;
};

#endif
