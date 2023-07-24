#include <Siv3D.hpp> // OpenSiv3D v0.6.10
#include <vector>
#include <complex>
#include <thread>
#include "FFT.h"
#include "LPC.h"
#include "GolombRiceCoding.h"
#include "Network.h"
#include "Mic.h"
#include "Debug.h"
#include "GraphPlot.h"
#include "constant.h"


VoiceNetwork::VoicePacket voice_packet = VoiceNetwork::VoicePacket();

void Main(){
	Scene::SetBackground(BACKGROUND_COLOR);
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

	const char* hostname = HOST_NAME;
	Mic::MicInput mic_input = Mic::MicInput(mic, SAMPLES_LENGTH);
	GraphPlot::SpectrumPlot spec_plot = GraphPlot::SpectrumPlot(Vec2{ 50, WINDOW_HEIGHT - 100}, 900, Palette::Black);
	VoiceNetwork::SendPacket send_packet = VoiceNetwork::SendPacket(hostname, PORT, IP_TYPE, PROTOCOL);

	int i = 0;

	while (System::Update()) {
		std::vector<std::complex<double>> f = mic_input.get_samples();
		std::vector<double> real_f = CodingProcess::get_complex_to_real_vector(f);
		VoiceNetwork::VoicePacket voice_packet = VoiceNetwork::VoicePacket();

		/* Plot Power Spectorum fft */
		CodingProcess::FFT fft = CodingProcess::FFT(f, sr);
		std::vector<double> freq = fft.get_FFT_frequency();
		std::vector<double> p_spec = fft.get_power_spectrum();
		//std::vector<double> p_spec = fft.get_amp_spectrum();
		spec_plot.plot(freq, p_spec);

		CodingProcess::LPCEncrypt lpc_e = CodingProcess::LPCEncrypt(real_f, LPC_COEFFICIENT_DIM);
		std::vector<double> pc = lpc_e.get_prediction_coefficient();
		std::vector<double> pe = lpc_e.get_predict_error();

		/* Golomb-Rice Encoding */
		CodingProcess::GolombRiceEncode gr_encode = CodingProcess::GolombRiceEncode(pe, GOLOMB_RICE_DIVISOR, GOLOMB_RICE_SCALE);
		std::vector<unsigned char> pe_encode = gr_encode.get_encode();
		std::vector<unsigned char> pc_encode = VoiceNetwork::convert_double_to_bytes(pc);
		Print << U"Golomb RIce Encode Length: " << pe_encode.size();
		voice_packet.pc = pc_encode;
		voice_packet.pe = pe_encode;
		voice_packet.pe_length = pe_encode.size();
		send_packet.send(voice_packet);

		/* Golomb-Rice Decoding */
		/*CodingProcess::GolombRiceDecode gr_decode = CodingProcess::GolombRiceDecode(pe_encode, GOLOMB_RICE_DIVISOR, GOLOMB_RICE_SCALE);
		std::vector<double> pe_decode = gr_decode.get_decode();*/

		/* LPC decrypt */
		/*CodingProcess::LPCDecrypt lpc_d = CodingProcess::LPCDecrypt(pc, pe);
		std::vector<double> lpc_f = lpc_d.get_f_decrypt();
		std::vector<std::complex<double>> comp_f = CodingProcess::get_real_to_complex_vector(lpc_f);*/
		//i++;
	}
	send_packet.close_socket();
}
