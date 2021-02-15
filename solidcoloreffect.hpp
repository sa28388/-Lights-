/*
Solid color effects
*/

#ifndef SOLID_EFFECTS_HPP
#define SOLID_EFFECTS_HPP

#include "colors.hpp"
#include "device.hpp"
#include "effectinterface.hpp"

class solidcolor : public effect {
private:
	colors::rgb _currcolor;
	float _brightness;
public:
	solidcolor() = delete;
	solidcolor(const colors::rgb& color, float brightness = 1.0f);
	~solidcolor();

	//change colors
	void setcolor(const colors::rgb& newcolor);

	//change brightness of the effect
	void setbrightness(float brightness);

	//apply the effect
	void apply(device& d) override;

	//deactivate the effect
	void deactivate(device& d) override;
};
#endif