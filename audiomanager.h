/*
Audio manager class that will allow us to see devices, start/stop
audio streams in port audio, select devices, and manage the audio processor, etc.
*/

#ifndef AUDIOMANAGER_HPP
#define AUDIOMANAGER_HPP

#include "audiostream.hpp"
#include <map>
#include <memory>
#include <portaudio.h>
#include <string>

class audiomanager {
private:
	std::unique_ptr<audiostream> _stream;
	std::map<std::string, int> _devices; //table of device name, and ID
	std::map<std::string, int>::const_iterator _currdevice; //iterator to current device in case we need it later
	PaStreamParameters _parameters; //Parameters for the stream given selected device

	void populatedevices();
	void refreshstream(); //restart/reopen the stream after device changes

public:
	audiomanager();
	~audiomanager();

	const std::map<std::string, int>& devices() const; //return list of devices
	void selectdevice(const std::string& devname);

	//Start and stop the audio stream (exposes stream's pause/start functions)
	void startstream(); 
	void pausestream();
	
};
#endif // !AUDIOMANAGER_HPP

