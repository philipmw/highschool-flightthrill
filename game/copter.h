// License: GNU General Public License v2

#ifndef _COPTER_H_
#define _COPTER_H_

typedef class Copter {
	public:
		Copter(int, int, int, int, int, int);
		// accessors
		unsigned int query_x() const;
		int query_y() const;
		unsigned int query_prot() const;
		// modifiers
		bool set_y(int);
		bool incr_x(int);
		bool incr_y(int);

		bool set_x_low(int);
		bool set_x_high(int);
		bool set_y_low(int);
		bool set_y_high(int);

		bool affect_prot(int);
		bool reset_prot();

		void apply_physics(bool, const int, const double);
		void reset_physics();
	private:
		// position
		unsigned int x_coord;
		int y_coord;
		unsigned int y_low, y_high;
		unsigned int x_low, x_high;
		// shields
		unsigned int protection;
		// physics
		bool direction;
		int time;
};

#endif
