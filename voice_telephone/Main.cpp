#include <Siv3D.hpp> // OpenSiv3D v0.6.10
#include <vector>
#include <complex>
#include "Mic.h"
#include "constant.h"

void Main(){
	if (System::EnumerateMicrophones().isEmpty()) {
		throw Error{U"No microphone is connected"};
	}

	const Microphone mic{ 5s, Loop::Yes, StartImmediately::Yes };
	if (not mic.isRecording()) {
		throw Error{U"Failed to start recording"};
	}

	Mic::MicInput mic_input = Mic::MicInput(mic, SAMPLES_LENGTH);

	while (System::Update()) {
		std::vector<std::complex<double>> wave = mic_input.get_samples();
	}

	delete &mic_input;
}
