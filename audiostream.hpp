/*
Class to manage audio stream input using Port Audio.
Since PA is written in C this will be a weird mix of C wrapped in C++
*/

#ifndef STREAM_HPP
#define STREAM_HPP

#include <portaudio.h> //Have to install this library, and dynamically link it (make sure the dll file is in the folder at runtime)

class audiostream {
private:
	//the port audio callback function that is called by PA at the interrupt level whenever it has received more audio.
	static int paRecordCallback(
		const void *inputBuffer, void *outputBuffer,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void *userData
	);

	PaStream* stream; //The stream object itself, made in constructor

public:
	audiostream(); //initialize stream (dont start)
	~audiostream(); //destruct and also stop the stream

	void start(); //start stream
	void stop(); //stop stream
};
#endif
