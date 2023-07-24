#pragma once

#include <Siv3D.hpp>
//#include <Windows.h>
//#include <locale.h>
#include <vector>
#include <complex>
#include "constant.h"

namespace AudioInOut {
	class MicInput {
	public:
		MicInput(Microphone mic, size_t samples_num);
		std::vector<std::complex<double>> get_samples();
	private:
		Microphone mic;
		size_t samples_num;
	};

	class SpeakerOutput {
	public:
		SpeakerOutput();
		void play(std::vector<double> audio_vec);
		void close_speaker();
	private:
		WAVEFORMATEX waveformatx;
		HWAVEOUT handle_wave_out;
		void setup_speaker();
	};
}
