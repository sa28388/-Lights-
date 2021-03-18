/*
Audio manager class that will allow us to see devices, start/stop
audio streams in port audio, select devices, and manage the audio processor, etc.
*/

#ifndef AUDIOMANAGER_HPP
#define AUDIOMANAGER_HPP

#include "audiostream.hpp"
#include <memory>
#include <portaudio.h>
#include <string>
#include <vector>

class audiomanager {
private:
	std::unique_ptr<audiostream> _stream;
	std::vector<std::string> _devices; //list of device names

	void populatedevices();

public:
	audiomanager();
	~audiomanager();

	const std::vector<std::string>& devices() const; //return list of devices


	
};
#endif // !AUDIOMANAGER_HPP

