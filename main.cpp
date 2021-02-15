/*
Device test
*/

#include <thread>
#include "device.hpp"
#include <iostream>
#include "solidcoloreffect.hpp"

int main() {
	std::string name("test"),
		ip("192.168.1.12");

	size_t nleds = 672,
		refresh = 60;

	device dev(name, ip, nleds, refresh);
	std::unique_ptr<effect> effect = std::make_unique<solidcolor>(colors::RED);

	dev.activate();
	dev.seteffect(std::move(effect));
	
	dev.start();

	return EXIT_SUCCESS;
}