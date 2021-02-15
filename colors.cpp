/*
Just defining some colors and a convenience function to create new ones
*/

#include "colors.hpp"

namespace colors {
	//convenience function
	rgb new_color(color_value r, color_value g, color_value b) {
		return std::make_tuple(r, g, b);
	}

	//pre-defined colors
	const rgb BLACK(0, 0, 0),
		WHITE(255, 255, 255),
		RED(255, 0, 0),
		GREEN(0, 255, 0),
		BLUE(0, 0, 255);
} //namespace