/*
Implementation of audiomanager
*/

#include "audiomanager.h"
#include <portaudio.h>

audiomanager::audiomanager() {
	Pa_Initialize(); //Initialize PA to generate device list. 
}

audiomanager::~audiomanager() {
	stream.stop();
	Pa_Terminate();
}
