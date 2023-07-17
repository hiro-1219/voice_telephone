﻿#include <Siv3D.hpp> // OpenSiv3D v0.6.10
#include <vector>
#include <complex>
#include "FFT.h"
#include "LPC.h"
#include "Mic.h"
#include "Debug.h"
#include "GraphPlot.h"
#include "constant.h"

void Main(){
	Window::Resize(WINDOW_WIDTH, WINDOW_HEIGHT);

	if (System::EnumerateMicrophones().isEmpty()) {
		throw Error{U"No microphone is connected"};
	}

#if DEBUG
	show_system_mic();
#endif

	const Microphone mic{ 2, 48000, 5s, Loop::Yes, StartImmediately::Yes };
	const size_t sr = mic.getSampleRate();
	if (not mic.isRecording()) {
		throw Error{U"Failed to start recording"};
	}

	Mic::MicInput mic_input = Mic::MicInput(mic, SAMPLES_LENGTH);
	GraphPlot::SpectrumPlot spec_plot = GraphPlot::SpectrumPlot(Vec2{ 50, WINDOW_HEIGHT - 50}, 900, Palette::White);
	GraphPlot::SpectrumPlot spec_plot2 = GraphPlot::SpectrumPlot(Vec2{ 50, WINDOW_HEIGHT - 100 }, 900, Palette::Aqua);

	while (System::Update()) {
		std::vector<std::complex<double>> f = mic_input.get_samples();
		std::vector<double> real_f = CodingProcess::get_complex_to_real_vector(f);

		/* Plot Power Spectorum fft */
		CodingProcess::FFT fft = CodingProcess::FFT(f, sr);
		std::vector<double> freq = fft.get_FFT_frequency();
		std::vector<double> p_spec = fft.get_power_spectrum();
		spec_plot.plot(freq, p_spec);
		
		/* LPC encrypt */
		CodingProcess::LPCEncrypt lpc_e = CodingProcess::LPCEncrypt(real_f, LPC_COEFFICIENT_DIM);
		std::vector<double> pc = lpc_e.get_prediction_coefficient();
		std::vector<double> pe = lpc_e.get_predict_error();

		/* LPC decrypt */
		CodingProcess::LPCDecrypt lpc_d = CodingProcess::LPCDecrypt(pc, pe);
		std::vector<double> lpc_f = lpc_d.get_f_decrypt();
		std::vector<std::complex<double>> comp_f = CodingProcess::get_real_to_complex_vector(lpc_f);


		/* Plot Power Spectorum fft2 */
		CodingProcess::FFT fft2 = CodingProcess::FFT(comp_f, sr);
		std::vector<double> freq2 = fft.get_FFT_frequency();
		std::vector<double> p_spec2 = fft.get_power_spectrum();
		spec_plot2.plot(freq2, p_spec2);
	}
}