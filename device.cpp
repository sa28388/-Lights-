/*
basically coding the behavior of a physical device
*/

#include <chrono>
#include "device.hpp"
#include <thread>

size_t device::_id = 0; //first ID will be 0.

device::device(const std::string& name, const std::string& ip, size_t nleds, size_t refreshrate) :
	_devid(_id++),
	_name(name),
	_activated(false),
	_haseffect(false),
	_ip(ip),
	_nleds(nleds),
	_refreshrate(refreshrate),
	_pixels(std::unique_ptr<ledstrip>(new ledstrip(nleds))),
	_currenteffect(nullptr), //no default effect
	_network(nullptr) //no connection at startup
{}

device::~device() {
	//disconnect and disable any effect.
	if (_activated) deactivate();
}

void device::flush() {
	//Send flattened pixels to the network handling thingy
	_network->flush(_pixels->flatten());
}
void device::activate() {
	//pointer to network handling object
	_network = std::make_unique<e131client>(_ip, _nleds);
	_activated = true;
}

void device::deactivate() {
	_network.reset(nullptr); //close network
	if (_haseffect) cleareffect(); //clear and clean up effects
	_activated = false;
}

void device::seteffect(std::unique_ptr<effect> effect) {
	if (_haseffect) cleareffect(); //clear any set effects first.
	_currenteffect = std::move(effect); //take the effect
	_haseffect = true; //we are controlling an effect now
}

void device::cleareffect() {
	//deactivate the effect and reset the pointer to it.
	_currenteffect->deactivate(*this);
	_currenteffect.reset(nullptr);
	_haseffect = false;
}

ledstrip& device::pixels() {
	//return our awesome data
	return *_pixels;
}

void device::start() {
	//time between frames in ms.
	float wait_time = (1 / static_cast<float>(_refreshrate)) * 1000;

	//chrono object to keep time for us
	std::chrono::duration<double, std::milli> d(wait_time);

	while (_haseffect) {
		//while the effect is active, send the current effect data to 
		//the device every d milliseconds
		_currenteffect->apply(*this); //apply the state of the effect to the device pixels
		_network->flush(_pixels->flatten()); //flush state to network
		std::this_thread::sleep_for(d); //wait for next frame
	}
}