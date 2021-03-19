/*
coding the behavior of a physical rgb led light

Is tuple adding extra unnecessary overhead?
*/

#ifndef LED_H
#define LED_H

#include "colors.hpp"
#include <tuple>

class led {
	using color_value = colors::color_value;
	using rgb = colors::rgb;

private:
	rgb _color; //currently set rgb color value, in a tuple
	float _brightness; //brightness scalar, and current brightness level (from 0 to 1)
	color_value _r, _g, _b; //effective color values - the actual output channels
	static color_value max; //shouldnt be more than 255 since thats how rgb works

	void refresh(); //refresh _r,_g,_b (the output channels)

public:
	led(); //everything will be initialized to default values. always.
	~led(); //destroy the color.

	led& operator=(const led& other); //copy assignment

	//Get and set brightness values
	//This can be a decimal, but since rgb code is all integral it will get casted/rounded.
	float brightness() const;
	void brightness(float brightness);

	//Get current color and set a new color
	rgb color() const;
	void color(const rgb& color);

	//Get and set individual RGB channels
	color_value r() const;
	void r(color_value r);

	color_value g() const;
	void g(color_value g);

	color_value b() const;
	void b(color_value b);

	void set(const rgb& color, float level); //set both color/brightness

	//Expose access to output channels. Const. Can't modify the state w/ these
	color_value r_out() const;
	color_value g_out() const;
	color_value b_out() const;
}; //led
#endif //LED_H