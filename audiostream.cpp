/*
Implementing audio stream listening.
Sorry for my inconsistent variable naming/cases :)))
*/

#include "audiostream.hpp"

// Sample format and audio settings for PortAudio
#define PA_SAMPLE_TYPE paFloat32
typedef float SAMPLE;
constexpr auto SAMPLE_SILENCE = (0.0f);
constexpr auto NUM_CHANNELS = 2;

typedef struct
{
	int frameindex;
	int maxframeindex;
	SAMPLE* recordedsamples;
} paTestData;

int audiostream::paRecordCallback(
	const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData)
{
	//organize our data structures / cast data passed through stream
	paTestData* data = (paTestData*)userData;
	const SAMPLE* rptr = (const SAMPLE*)inputBuffer;
	SAMPLE* wptr = &data->recordedsamples[data->frameindex * NUM_CHANNELS];

	int finished; //did we read all the data from the buffer?
	long framestocalc, i; 
	unsigned long framesleft = data->maxframeindex - data->frameindex;

	//prevent some unused variable warnings, since we are recording 
	//we don't use the output buffer etc.
	(void)outputBuffer;
	(void)timeInfo;
	(void)statusFlags;
	(void)userData;

	if (framesleft < framesPerBuffer) {
		framestocalc = framesleft;
		finished = paComplete;
	}
	else {
		framestocalc = framesPerBuffer;
		finished = paContinue;
	}

	if (inputBuffer == nullptr) { //if empty
		for (i = 0; i < framestocalc; ++i) {
			//then write silence to 2 spots/channels to our userData object
			*wptr++ = SAMPLE_SILENCE;
			if (NUM_CHANNELS == 2) *wptr++ = SAMPLE_SILENCE;
		}
	}
	else {
		for (i = 0; i < framestocalc; ++i) {
			*wptr++ = *rptr++; //left channel
			if (NUM_CHANNELS == 2) *wptr++ = *rptr++; //right channel
		}
	}
	data->frameindex += framestocalc;
	return finished;
}

audiostream::audiostream() {}

audiostream::~audiostream() {
	stop(); //make sure to stop the stream if it's active.
}

void audiostream::start() {
	Pa_StartStream(stream);
}

void audiostream::stop() {
	if (Pa_IsStreamActive(stream)) Pa_StopStream(stream);
}