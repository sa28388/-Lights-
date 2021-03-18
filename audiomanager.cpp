/*
Implementation of audiomanager
*/

#include "audiomanager.h"
#include <iostream>
#include <portaudio.h>

audiomanager::audiomanager() {
	Pa_Initialize(); //Initialize PA to generate device list. 
	populatedevices();
}

audiomanager::~audiomanager() {
	if (_stream) _stream->stop();
	Pa_Terminate();
}

void audiomanager::populatedevices() {
	int ndevices = Pa_GetDeviceCount(), i = 0;

	_devices.reserve(ndevices);

	for (i; i != ndevices; ++i) {
		_devices[i] = Pa_GetDeviceInfo(i)->name;
		std::cout << _devices[i] << std::endl;
	}
}

const std::vector<std::string>& audiomanager::devices() const {
	return _devices;
}
