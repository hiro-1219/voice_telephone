#include <Siv3D.hpp>
#include <vector>
#include <complex>
#include "Mic.h"

Mic::MicInput::MicInput(Microphone mic, size_t samples_num) {
	this->mic = mic;
	this->samples_num = samples_num;
}


std::vector<std::complex<double>> Mic::MicInput::get_samples() {
	std::vector<std::complex<double>> f(this->samples_num);
	const Wave& wave = this->mic.getBuffer();
	const size_t writePos = mic.posSample();
	const size_t headLength = Min(writePos, this->samples_num);
	const size_t tailLength = this->samples_num - headLength;
	int pos = 0;
	for (int i = 0; i < tailLength; i++) {
		f[pos] = std::complex<double>(wave[wave.size() - tailLength + i].left, 0);
		pos++;
	}
	for (int i = 0; i < headLength; i++) {
		f[pos] = std::complex<double>(wave[writePos - headLength + i].left, 0);
		pos++;
	}
	return f;
}
