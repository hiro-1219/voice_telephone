#pragma once
#include <Siv3D.hpp>
#include <vector>
#include <complex>

namespace Mic {
	class MicInput {
	public:
		MicInput(Microphone mic, size_t samples_num);
		std::vector<std::complex<double>> get_samples();
	private:
		Microphone mic;
		size_t samples_num;
	};
}
