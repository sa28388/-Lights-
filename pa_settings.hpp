/*
settings for port audio streams etc.
*/

#ifndef PA_SETTINGS_HPP
#define PA_SETTINGS_HPP

// Sample format and audio settings for PortAudio stream
#define PA_SAMPLE_TYPE paFloat32
typedef float SAMPLE;
constexpr auto SAMPLE_SILENCE = (0.0f);
constexpr auto NUM_CHANNELS = 2;
constexpr auto SAMPLE_RATE = 44100;
constexpr auto FRAMES_PER_BUF = 256;

#endif