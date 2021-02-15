/*
basically coding the behavior of a physical device
*/

#ifndef DEVICE_H
#define DEVICE_H

#include "e131client.hpp"
#include "effect.hpp"
#include "ledstrip.hpp"
#include <memory>
#include <string>

class device {
private:
	static size_t _id; //unique ID for each device, object level
	
	bool _activated, _haseffect;
	size_t _devid, _nleds, _refreshrate; //device ID (for this device), size and device-specific refresh rate
	std::string _name, _ip; //device name and IP address
	std::unique_ptr<ledstrip> _pixels; //the associated led strip
	std::unique_ptr<effect> _currenteffect; //current associated effect (defaults to none)
	std::unique_ptr<e131client> _network; //current network "device". keep open to maybe support other types later

	void flush(); //flush data to network

public:
	device() = delete;
	device(const std::string& name, const std::string& ip, size_t nleds, size_t refreshrate);
	~device();

	void activate(); //start network. does NOT set a default effect
	void deactivate(); //close network and clear current effect

	void seteffect(std::unique_ptr<effect> effect); //set an effect. by value since this takes ownership
	void cleareffect(); //clear an effect

	//ledstrip access functions. ledstrip has operator[] for led-level control.
	ledstrip& pixels(); //make pixels available to callers/effects. Also gives access to led level brightness

	void start(); //main run loop
};
#endif //!DEVICE_H