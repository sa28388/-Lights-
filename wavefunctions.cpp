/*
Defining some functions to control brightness waves
*/

#include <cmath>
#include "wavefunctions.hpp"

namespace functions {
	const wavefunction TRIANGULAR = [](float i, float smoothness, float dummy1, float dummy2) {
		return static_cast<float>((1.0 - abs((2.0*(i / smoothness)) - 1.0)));
	};

	const wavefunction CIRCULAR = [](float i, float smoothness, float dummy1, float dummy2) {
		return static_cast<float>(sqrt(1.0 - pow(abs((2.0*(i / smoothness)) - 1.0), 2.0)));
	};

	const wavefunction GAUSSIAN = [](float i, float smoothness, float gamma, float beta) {
		return static_cast<float>(exp(-(pow(((i / smoothness) - beta) / gamma, 2.0)) / 2.0));
	};
}