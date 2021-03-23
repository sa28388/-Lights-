/*
Class to manage audio stream input using Port Audio.

How this works:
The static function paRecordCallback is what portaudio engine calls at the interrupt level. Don't do complex stuff there.
Since we have a C++ class, and not a free data struct, we need to pass the callback our buffer/stream pointers. 
Since the callback is static, it can't access the data members normally. So, we simply
have the callback call a non-static member function (same name) that does the actual callback.
We can pass 'this' object into the static function, and call its functions.
The function simply pulls data from the portaudio inputstream object and writes it to the ring buffer.
Also have a function to initialize the ringbuffer, which has to be done before the stream is started.

*/

#ifndef STREAM_HPP
#define STREAM_HPP

#include <portaudio.h> //Have to install this library, and dynamically link it (make sure the dll file is in the folder at runtime)
#include "pa_ringbuffer.h" //ring buffer we will record audio to

class audiostream {
private:
	PaStream* _stream; //The stream object itself, parameterized in constructor
	PaUtilRingBuffer _buffer; //the ring buffer for r/w
	void* _bufferdata; //this will keep the address of the buffer data so we can free it on destruction

	//need this so the static function calls it, since static can't access data members
	int _parecordcallback(const void* input,
		const void* output,
		unsigned long frameCount,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlag);

	void _bufferinit(); //Initialize the buffer

public:
	//the port audio callback function that is called by PA at the interrupt level whenever it has received more audio.
	//Don't do anything that can mess up the system like malloc() or free().
	static int paRecordCallback(
		const void* inputBuffer, void* outputBuffer,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void* userData);

	audiostream(); //initialize/open stream (dont start). Default recording device
	audiostream(const PaStreamParameters* deviceParameters); //Open stream with specific recording device parameters
	~audiostream(); //destruct and also stop the stream

	void start(); //start stream
	void stop(); //stop stream

	PaUtilRingBuffer* buffer(); //Buffer read access
};
#endif
