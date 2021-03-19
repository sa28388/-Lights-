/*
coding the behavior of a physical led strip
*/

#include "ledstrip.hpp"

void ledstrip::_flatten() {
	//serialize the current state of output channels into one long array
	int i = 0;
	for (auto& led : _pixels) {
		_flatpixels[i++] = led.r_out();
		_flatpixels[i++] = led.g_out();
		_flatpixels[i++] = led.b_out();
	}
}

ledstrip::ledstrip(size_t nlights) :
	_pixels(nlights),
	_flatpixels(nlights * 3, 0), //3x length when we serialize all channels
	_length(nlights)
{}

ledstrip::~ledstrip() {}

size_t ledstrip::size() const { return _length; }

std::vector<int>& ledstrip::flatten() {
	//We will update the currently stored flat version first, in our 
	//preallocated array.
	_flatten(); 
	return _flatpixels;
}

std::vector<led>& ledstrip::array() {
	return _pixels;
}

led& ledstrip::operator[](size_t i) {
	return _pixels[i];
}

//These functions simply take advantage of LED-level control to manipulate the whole strip.
void ledstrip::setcolor(const rgb& color) {
	for (auto& pixel : _pixels) pixel.color(color); //set each pixel to color
}

void ledstrip::setbrightness(float level) {
	for (auto& pixel : _pixels) pixel.brightness(level); //set each pixel to brightness level
}

void ledstrip::set(const rgb& color, float level) {
	for (auto& pixel : _pixels) pixel.set(color, level);
}