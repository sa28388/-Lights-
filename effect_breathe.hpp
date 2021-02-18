#ifndef BREATHE_EFFECT_HPP
#define BREATHE_EFFECT_HPP

#include <atomic>
#include "colors.hpp"
#include "device.hpp"
#include "effect_interface.hpp"
#include <functional>
#include <mutex>
#include <thread>
#include "wavefunctions.hpp"

class breathe : public effect {
private:
	using wavefunction = std::function<float(float, float, float, float)>;

	colors::rgb _color;
	float _brightness, //current brightness value
		_beta, //beta and gamma are parameters used ONLY in gaussian wave function
		_gamma,
		_smoothness; //bigger = faster brightness change
	wavefunction _w; //function to calculate the next brightness level
	
	//threading stuff
	std::thread _t; // thread running this effect
	std::atomic_bool _alive; //so we can kill the thread later
	std::mutex mut; //Lock this object when it gets modified

	void process();

public:
	breathe() = delete;
	breathe(const colors::rgb& color, float smoothness = 500, wavefunction w = functions::GAUSSIAN);
	~breathe();

	void setcolor(const colors::rgb& newcolor); //change color

	void apply(device& d) override; //send the state to a device

	void deactivate(device& d) override; //turn off

	//Gamma and beta are 2 parameters that effect the gaussian curve. See wavefunction definition.
	void gamma(float g); //set gamma

	void beta(float b); //set beta

	void speed(float speed); //set smoothness/speed. Bigger = smoother/slower

	void wavefunc(wavefunction w); //change the function used for the waveform
};
#endif // !BREATHE_EFFECT_HPP

