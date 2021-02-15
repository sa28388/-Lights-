/*
coding the behavior of a physical led strip
*/

#ifndef LEDSTRIP_H
#define LEDSTRIP_H

#include "led.hpp"
#include <vector>

class ledstrip {
private:
	using rgb = std::tuple<size_t, size_t, size_t>;

	std::vector<led> _pixels; //and RGB array ((rgb),(rgb),(rgb),...)
	std::vector<int> _flatpixels; //a flattened version of the array (3x the size). Preallocated memory that we re-use each call
	size_t _length; //how many lights is the strip

	//flatten the rgb array into a 1D sequential array (rgbrgbrgb...)
	void _flatten();

public:
	ledstrip() = delete;
	ledstrip(size_t nlights);
	~ledstrip();

	//strip size
	size_t size() const;

	//Return a reference to the flattened array.
	//We need this to populate the dmx data of the e131 packet
	std::vector<int>& flatten();

	//Access the individual leds of strip by reference so we can modify it.
	led& operator[](size_t i);

	//conveniently set color of an entire strip
	void setcolor(const rgb& color);

	//set brightness of entire strip
	void setbrightness(float level);

}; //ledstrip
#endif //LEDSTRIP_H