/*
Solid color effects
*/

#include "effect_solidcolor.hpp"

solidcolor::solidcolor(const colors::rgb& color, float brightness) :
	_currcolor(color),
	_brightness(brightness) //Default to 100% brightness
{}

solidcolor::~solidcolor() {}

void solidcolor::setcolor(const colors::rgb& newcolor) {
	_currcolor = newcolor;
}

void solidcolor::setbrightness(float brightness) { _brightness = brightness; }

void solidcolor::apply(device& d) {
	//set the color and brightness to the whole strip
	d.pixels().setbrightness(_brightness);
	d.pixels().setcolor(_currcolor);
}

void solidcolor::deactivate(device& d) {
	//set color to black to turn it off.
	d.pixels().setcolor(colors::BLACK);
}
