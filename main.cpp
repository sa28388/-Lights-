/*
Device test
*/

#include <thread>
#include "device.hpp"
#include <iostream>
#include "effect_solidcolor.hpp"
#include "effect_breathe.hpp"

int main() {
	std::string name("test"),
		ip("192.168.1.12");

	size_t nleds = 672,
		refresh = 60;

	device dev(name, ip, nleds, refresh);
	
	
	dev.activate();
	dev.seteffect(std::make_unique<breathe>(colors::RED));
	
	dev.start();

	return EXIT_SUCCESS;
}