/*
coding the behavior of a physical rgb led light

Is tuple adding extra unnecessary overhead?
*/

#ifndef LED_H
#define LED_H

#include <tuple>

class led {
	using color_value = size_t;
	using rgb = std::tuple<color_value, color_value, color_value>;

private:
	rgb _color; //currently set rgb color value, in a tuple
	float _brightness; //brightness scalar, and current brightness level (from 0 to 1)
	color_value _r, _g, _b, //effective color values - the actual output channels
		max; //shouldnt be more than 255 since thats how rgb works

	void refresh(); //refresh _r,_g,_b (the output channels)

public:
	led(); //everything will be initialized to default values. always.
	~led(); //destroy the color.

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

	//Expose access to output channels. Const. Can't modify the state w/ these
	color_value r_out() const;
	color_value g_out() const;
	color_value b_out() const;
}; //led
#endif //LED_H