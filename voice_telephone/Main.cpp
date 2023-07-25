#include <Siv3D.hpp> // OpenSiv3D v0.6.10
#include <vector>
#include <complex>
#include <thread>
#include "FFT.h"
#include "LPC.h"
#include "GolombRiceCoding.h"
#include "Network.h"
#include "AudioInOut.h"
#include "Debug.h"
#include "GraphPlot.h"
#include "constant.h"


VoiceNetwork::VoicePacket voice_packet = VoiceNetwork::VoicePacket();
std::vector<double> play_f;

void speaker_output_thread(AudioInOut::SpeakerOutput speaker_out, int sr) {
	VoiceNetwork::RecvPacket recv_packet = VoiceNetwork::RecvPacket(RECV_PORT);
	while (1) {
		VoiceNetwork::VoicePacket voice_packet = recv_packet.recv();
		std::vector<double> pc_decode = VoiceNetwork::convert_bytes_to_double(voice_packet.pc);

		/* Golomb-Rice Decoding */
		CodingProcess::GolombRiceDecode gr_decode = CodingProcess::GolombRiceDecode(voice_packet.pe, GOLOMB_RICE_DIVISOR, GOLOMB_RICE_SCALE);
		std::vector<double> pe_decode = gr_decode.get_decode();

		/* LPC decrypt */
		CodingProcess::LPCDecrypt lpc_d = CodingProcess::LPCDecrypt(pc_decode, pe_decode);
		play_f = lpc_d.get_f_decrypt();

		/* speaker */
		if (!play_f.empty() && PLAY) {
			speaker_out.play(play_f);
		}
	}
}

void mic_input_thread(AudioInOut::MicInput mic_input, int sr) {

	const char* hostname = HOST_NAME;
	GraphPlot::SpectrumPlot spec_plot = GraphPlot::SpectrumPlot(Vec2{ 50, WINDOW_HEIGHT - 100 }, 900, Palette::Black);
	GraphPlot::SpectrumPlot play_plot = GraphPlot::SpectrumPlot(Vec2{ 50, WINDOW_HEIGHT - 80 }, 900, Palette::Red);

	VoiceNetwork::SendPacket send_packet = VoiceNetwork::SendPacket(hostname, SEND_PORT);

	while (System::Update()) {
		std::vector<std::complex<double>> f = mic_input.get_samples();
		std::vector<double> real_f = CodingProcess::get_complex_to_real_vector(f);
		VoiceNetwork::VoicePacket voice_packet = VoiceNetwork::VoicePacket();

		/* Plot Power Spectorum fft */
		CodingProcess::FFT fft = CodingProcess::FFT(f, sr);
		std::vector<double> freq = fft.get_FFT_frequency();
		std::vector<double> p_spec = fft.get_power_spectrum();
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

		if (!play_f.empty()) {
			std::vector<std::complex<double>> play_f_complex = CodingProcess::get_real_to_complex_vector(play_f);
			CodingProcess::FFT fft_play = CodingProcess::FFT(play_f_complex, sr);
			std::vector<double> freq_play = fft_play.get_FFT_frequency();
			std::vector<double> p_spec_play = fft_play.get_power_spectrum();
			play_plot.plot(freq_play, p_spec_play);
		}
	}
	send_packet.close_socket();
}

void Main(){
	Scene::SetBackground(BACKGROUND_COLOR);
	Window::Resize(WINDOW_WIDTH, WINDOW_HEIGHT);

	if (System::EnumerateMicrophones().isEmpty()) {
		throw Error{U"No microphone is connected"};
	}

#if DEBUG
	Debug::show_system_mic();
#endif

	const Microphone mic{ MIC_INDEX, SAMPLE_RATE, 5s, Loop::Yes, StartImmediately::Yes };
	const size_t sr = mic.getSampleRate();
	if (not mic.isRecording()) {
		throw Error{U"Failed to start recording"};
	}
	AudioInOut::MicInput mic_input = AudioInOut::MicInput(mic, SAMPLES_LENGTH);
	AudioInOut::SpeakerOutput speaker_out = AudioInOut::SpeakerOutput();

	VoiceNetwork::SetupWSAStartup wsa_setup = VoiceNetwork::SetupWSAStartup();
	std::thread micInputThread(mic_input_thread, mic_input, sr);
	std::thread speakerOutputThread(speaker_output_thread, speaker_out, sr);
	micInputThread.join();
	speakerOutputThread.join();
	speaker_out.close_speaker();
	wsa_setup.close_wsa();
}
