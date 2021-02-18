/*
Implement the breathe effect
*/

#include "effect_breathe.hpp"

breathe::breathe(const colors::rgb& color, float smoothness, wavefunction w) :
	_color(color),
	_brightness(0.0f),
	_beta(0.5f),
	_gamma(0.14f),
	_smoothness(smoothness),
	_w(w),
	_alive(true),
	_t(&breathe::process, this) //start the thread!
{}

void breathe::process() {
	while (_alive) {
		for (int i = 0; i < _smoothness && _alive; ++i) {
			using namespace std::literals;
			std::this_thread::sleep_for(5ms);
			std::lock_guard<std::mutex> lock(mut); //block modification (no color, function, etc. changes)
			_brightness = _w((float)i, _smoothness, _gamma, _beta); //get new brightness from waveform function
		} //lock automatically released here, now any threads waiting can change color etc.
	} 
}

breathe::~breathe() {
	//stop breathing
	_alive = false;
	_t.join(); 
}

void breathe::setcolor(const colors::rgb& newcolor) {
	std::lock_guard<std::mutex> lock(mut);
	_color = newcolor;
}

void breathe::apply(device& d) {
	std::lock_guard<std::mutex> lock(mut);
	d.pixels().set(_color, _brightness);
}

void breathe::deactivate(device& d) {
	d.pixels().set(colors::BLACK, 0);
}

void breathe::gamma(float g) { 
	std::lock_guard<std::mutex> lock(mut);
	_gamma = g; 
}

void breathe::beta(float b) {
	std::lock_guard<std::mutex> lock(mut);
	_beta = b;
}

void breathe::speed(float speed) { 
	std::lock_guard<std::mutex> lock(mut);
	_smoothness = speed;
}
void breathe::wavefunc(wavefunction w) {
	std::lock_guard<std::mutex> lock(mut);
	_w = w;

}