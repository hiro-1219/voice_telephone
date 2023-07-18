#include <Siv3D.hpp> // OpenSiv3D v0.6.10
#include <vector>
#include <complex>
#include "FFT.h"
#include "LPC.h"
#include "GolombRiceCoding.h"
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
	Debug::show_system_mic();
#endif

	const Microphone mic{ MIC_INDEX, 48000, 5s, Loop::Yes, StartImmediately::Yes };
	const size_t sr = mic.getSampleRate();
	if (not mic.isRecording()) {
		throw Error{U"Failed to start recording"};
	}

	Mic::MicInput mic_input = Mic::MicInput(mic, SAMPLES_LENGTH);
	GraphPlot::SpectrumPlot spec_plot = GraphPlot::SpectrumPlot(Vec2{ 50, WINDOW_HEIGHT - 50}, 900, Palette::White);
	GraphPlot::SpectrumPlot spec_plot2 = GraphPlot::SpectrumPlot(Vec2{ 50, WINDOW_HEIGHT - 100 }, 900, Palette::Aqua);
	//GraphPlot::AudioPlot audio_plot = GraphPlot::AudioPlot(Vec2{ 50, (int)(WINDOW_HEIGHT / 2) }, 900, Palette::Green);

	while (System::Update()) {
		std::vector<std::complex<double>> f = mic_input.get_samples();
		std::vector<double> real_f = CodingProcess::get_complex_to_real_vector(f);

		/* Plot Power Spectorum fft */
		/*CodingProcess::FFT fft = CodingProcess::FFT(f, sr);
		std::vector<double> freq = fft.get_FFT_frequency();
		//std::vector<double> p_spec = fft.get_power_spectrum();
		std::vector<double> p_spec = fft.get_amp_spectrum();
		spec_plot.plot(freq, p_spec);*/

		
		/* LPC encrypt */
		CodingProcess::LPCEncrypt lpc_e = CodingProcess::LPCEncrypt(real_f, LPC_COEFFICIENT_DIM);
		std::vector<double> pc = lpc_e.get_prediction_coefficient();
		std::vector<double> pe = lpc_e.get_predict_error();

		//std::vector<double> time_vec = GraphPlot::get_arrange_vec(0, SAMPLES_LENGTH, SAMPLES_LENGTH);
		//audio_plot.plot(time_vec, pe);

		/* Golomb-Rice Encoding */
		/*CodingProcess::GolombRiceEncode gr_encode = CodingProcess::GolombRiceEncode(pe, GOLOMB_RICE_DIVISOR, GOLOMB_RICE_SCALE);
		std::vector<unsigned char> pe_encode = gr_encode.get_encode();*/
		//Print << U"Golomb RIce Encode Length: " << pe_encode.size();

		/* Golomb-Rice Decoding */
		/*CodingProcess::GolombRiceDecode gr_decode = CodingProcess::GolombRiceDecode(pe_encode, GOLOMB_RICE_DIVISOR, GOLOMB_RICE_SCALE);
		std::vector<double> pe_decode = gr_decode.get_decode();*/

		/* LPC decrypt */
		CodingProcess::LPCDecrypt lpc_d = CodingProcess::LPCDecrypt(pc, pe);
		std::vector<double> lpc_f = lpc_d.get_f_decrypt();
		std::vector<std::complex<double>> comp_f = CodingProcess::get_real_to_complex_vector(lpc_f);


		/* Plot Power Spectorum fft2 */
		CodingProcess::FFT fft2 = CodingProcess::FFT(comp_f, sr);
		std::vector<double> freq2 = fft2.get_FFT_frequency();
		//std::vector<double> p_spec2 = fft2.get_power_spectrum();
		std::vector<double> p_spec2 = fft2.get_amp_spectrum();
		spec_plot2.plot(freq2, p_spec2);
	}
}
