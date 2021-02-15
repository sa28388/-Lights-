/*
Just defining some colors and a convenience function to create them
*/

#ifndef COLORS_HPP
#define COLORS_HPP

#include <tuple>

namespace colors {
	using color_value = size_t;
	using rgb = std::tuple<color_value, color_value, color_value>;

	//easily create new colors later
	rgb new_color(color_value r, color_value g, color_value b);

	//predefined colors
	extern const rgb BLACK, //We can use these definitions in many files.
		WHITE,
		RED,
		GREEN,
		BLUE;
} //namespace

#endif // !COLORS_HPP
