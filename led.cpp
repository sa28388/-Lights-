/*
coding the behavior of a physical rgb led light
*/

#include <algorithm>
#include "led.hpp"

void led::refresh() {
	//refresh output channels 
	//apply the brightness/adjusted output.
	_r = std::min(static_cast<color_value>(std::get<0>(_color) * _brightness), max); //limit to 255
	_g = std::min(static_cast<color_value>(std::get<1>(_color) * _brightness), max);
	_b = std::min(static_cast<color_value>(std::get<2>(_color) * _brightness), max);
}

led::led() :
	_color(0, 0, 0),
	_brightness(0),
	_r(0),
	_g(0),
	_b(0),
	max(255)
{}

led::~led() {}

float led::brightness() const { return _brightness; }
void led::brightness(float brightness) {
	_brightness = brightness > 1.0f ? 1.0f : brightness; //max brightness is 100%
	refresh(); //apply new brightness setting
}

led::rgb led::color() const { return _color; }

void led::color(const rgb& color) {
	_color = color;
	refresh(); //refresh the strip after every change in input channel
}

//get and set individual rgb channels. 
led::color_value led::r() const { return std::get<0>(_color); }
void led::r(color_value r) {
	std::get<0>(_color) = r;
	refresh();
}

led::color_value led::g() const { return std::get<1>(_color); }
void led::g(color_value g) {
	std::get<1>(_color) = g;
	refresh();
}

led::color_value led::b() const { return std::get<2>(_color); }
void led::b(color_value b) {
	std::get<2>(_color) = b;
	refresh();
}

//set color and brightness together
void led::set(const rgb& color, float level) {
	_color = color;
	brightness(level); //this will call refresh
}

//Expose access to output channels. 
//We can't use these to actually modify the channels themselves. Observation ONLY
led::color_value led::r_out() const { return _r; }
led::color_value led::g_out() const { return _g; }
led::color_value led::b_out() const { return _b; }