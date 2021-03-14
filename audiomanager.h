/*
Audio manager class that will allow us to see devices, start/stop
audio streams in port audio, select devices, etc.
*/

#ifndef AUDIOMANAGER_HPP
#define AUDIOMANAGER_HPP

#include "audiostream.hpp"

class audiomanager {
private:
	audiostream stream;
public:
	audiomanager();
	~audiomanager();

	
};
#endif // !AUDIOMANAGER_HPP

