/*
Implementing audio stream listening.
Sorry for my inconsistent variable naming/cases :)))
Since PA is written in C this will be a weird (bad) mix of C wrapped in C++.
Hide all the ugly mess inside this class

Another note: Port Audio doesn't normally expose the ringbuffer structure it provides in the library. So we have to add
the ring buffer C code to our project separately. Same for the memory management functions. However the memory management
functions have OS specific .c files for implementation. They're not part of the public API... 
*/

#include "audiostream.hpp"
#include <algorithm>
#include "pa_settings.hpp"
#include <pa_util.h> //for memory management of ring buffer

//PA calls this function at the interrupt level. Don't do fancy stuff here like mutex/locks/etc. that could
//mess up the callback priority. 
int audiostream::paRecordCallback(
	const void* inputBuffer, void* outputBuffer, //the input and output streams of PA. (where it records/sends samples)
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void* userData) //we will pass 'this' object as it contains our userdata - the stream itself and the ring buffer
{
	//just call the member function, so the we have access to all the class data.
	//Casting pointers scares me :)
	return ((audiostream*)userData)->_parecordcallback(inputBuffer, outputBuffer, framesPerBuffer, timeInfo, statusFlags);
}

void audiostream::_bufferinit() {
	//The buffer size has to be allocated to a power of 2, so this will be convenient.
	auto nextpowerof2 = [](unsigned val) -> unsigned {
		val--;
		val = (val >> 1) | val;
		val = (val >> 2) | val;
		val = (val >> 4) | val;
		val = (val >> 8) | val;
		val = (val >> 16) | val;
		return ++val;
	};
	
	//This will initialize the ring buffer to hold about 500ms of audio according to the portaudio docs
	//Calculate how much memory we need to allocate
	unsigned numsamples = nextpowerof2((unsigned)(SAMPLE_RATE *0.5*NUM_CHANNELS));
	unsigned numbytes = numsamples * sizeof(SAMPLE);

	//allocate the memory
	SAMPLE* ringbuffptr = (SAMPLE*)PaUtil_AllocateMemory(numbytes);

	//initialize the buffer
	PaUtil_InitializeRingBuffer(&_buffer, sizeof(SAMPLE), numsamples, ringbuffptr);

	//store the pointer so we can free the mem later
	_bufferdata = (void*)ringbuffptr;
}

//the "real" callback that does the work of recording and putting into a buffer.
//This function has access to the stream and buffer in this object
//Essentially same signature as callback, but w/ no userData param since we called this function from userData.
int audiostream::_parecordcallback(const void* input, //same as inputbuffer 
	const void* output, //same as output buffer
	unsigned long frameCount,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlag)
{
	//No unused variable warnings
	(void)output; //We don't need this as we're not sending audio to PA. Only recording from it.
	(void)frameCount; //Dont care about these either
	(void)timeInfo;
	(void)statusFlag;

	//ring_buffer_size_t is a long. First calculate how many samples we can write into the buffer
	ring_buffer_size_t elementswriteable = PaUtil_GetRingBufferWriteAvailable(&_buffer);
	ring_buffer_size_t elementstowrite = std::min(elementswriteable, (ring_buffer_size_t)(FRAMES_PER_BUF*NUM_CHANNELS));
	const SAMPLE* rptr = (const SAMPLE*)input; //read pointer to read from the PA input stream

	//now write to the ring buffer
	PaUtil_WriteRingBuffer(&_buffer, rptr, elementstowrite);

	return paContinue;
}

audiostream::audiostream() {
	//make sure the buffer is initialized
	_bufferinit();

	//open the default stream for recording
	Pa_OpenDefaultStream(&_stream,
		NUM_CHANNELS, //2 (default) input channels
		0, //no output channels
		PA_SAMPLE_TYPE,
		SAMPLE_RATE,
		FRAMES_PER_BUF, 
		paRecordCallback, //static callback function
		(void*)this); //The data structure that contains the stream data is this object.
}

audiostream::~audiostream() {
	stop(); //make sure to stop the stream if it's active.
	PaUtil_FreeMemory((SAMPLE*)_bufferdata);
}

void audiostream::start() {
	Pa_StartStream(_stream);
}

void audiostream::stop() {
	if (Pa_IsStreamActive(_stream)) Pa_StopStream(_stream);

	//Additionally, flush buffer
	PaUtil_FlushRingBuffer(&_buffer);
}

PaUtilRingBuffer* audiostream::buffer() {
	return &_buffer;
}