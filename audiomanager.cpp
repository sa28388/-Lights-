/*
Implementation of audiomanager
*/

#include "audiomanager.h"
#include <iostream>
#include "pa_settings.hpp"
#include <portaudio.h>

audiomanager::audiomanager() : _stream(new audiostream()) {
	Pa_Initialize(); //Initialize PA to generate device list. 
	populatedevices(); //Store the device list for convenience
}

audiomanager::~audiomanager() {
	if (_stream) _stream->stop();
	Pa_Terminate();
}

void audiomanager::populatedevices() {
	int ndevices = Pa_GetDeviceCount(), i = 0;
	const PaDeviceInfo* deviceInfo;

	for (i; i < ndevices; ++i) {
		deviceInfo = Pa_GetDeviceInfo(i);
		_devices[deviceInfo->name] = i;
	}

	_currdevice = _devices.cend(); //just set iterator to cend to start
}

const std::map<std::string, int>& audiomanager::devices() const {
	return _devices;
}

void audiomanager::selectdevice(const std::string& devname) {
	_currdevice = _devices.find(devname);
	refreshstream();
}

void audiomanager::refreshstream() {
	//First get the device parameters for the selected device
	//Lookup the device ID based on the name in the devices table using the currdevice iterator
	const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo(_currdevice->second); 

	//Now, update the input stream parameters with that device's information
	_parameters.channelCount = deviceInfo->maxInputChannels;
	_parameters.device = _currdevice->second; //Device ID number
	_parameters.hostApiSpecificStreamInfo = nullptr; //I have no idea what this does, tbh
	_parameters.suggestedLatency = deviceInfo->defaultLowInputLatency;
	_parameters.sampleFormat = PA_SAMPLE_TYPE;

	//Pause, close, and reopen the stream (does not start listening to stream again)
	pausestream();
	_stream.reset(new audiostream(&_parameters));
}

void audiomanager::pausestream() {
	_stream->stop();
}

void audiomanager::startstream() {
	_stream->start();
}
