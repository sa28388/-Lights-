/*
Defining some functions to control brightness waves
*/

#ifndef WAVEFUNCTIONS_HPP
#define WAVEFUNCTIONS_HPP

#include <functional>

namespace functions {
	using wavefunction = std::function<float(float, float, float, float)>;

	extern const wavefunction
		TRIANGULAR,
		CIRCULAR,
		GAUSSIAN;
}
#endif