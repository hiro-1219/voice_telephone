#include <Siv3D.hpp> // OpenSiv3D v0.6.10
#include <vector>
#include <complex>
#include "FFT.h"
#include "Mic.h"
#include "GraphPlot.h"
#include "constant.h"

void Main(){

	Window::Resize(WINDOW_WIDTH, WINDOW_HEIGHT);

	if (System::EnumerateMicrophones().isEmpty()) {
		throw Error{U"No microphone is connected"};
	}

	const Microphone mic{ 5s, Loop::Yes, StartImmediately::Yes };
	const size_t sr = mic.getSampleRate();
	if (not mic.isRecording()) {
		throw Error{U"Failed to start recording"};
	}

	Mic::MicInput mic_input = Mic::MicInput(mic, SAMPLES_LENGTH);
	GraphPlot::SpectrumPlot spec_plot = GraphPlot::SpectrumPlot(Vec2{ 50, WINDOW_HEIGHT - 100}, 900);

	while (System::Update()) {
		std::vector<std::complex<double>> f = mic_input.get_samples();
		CodingProcess::FFT fft = CodingProcess::FFT(f, sr);
		std::vector<double> freq = fft.get_FFT_frequency();
		std::vector<double> p_spec = fft.get_power_spectrum();
		spec_plot.plot(freq, p_spec);
	}
}
