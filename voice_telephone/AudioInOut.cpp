#include <Siv3D.hpp>
#include <vector>
#include <complex>
#include "AudioInOut.h"
#include <Windows.h>

AudioInOut::MicInput::MicInput(Microphone mic, size_t samples_num) {
	this->mic = mic;
	this->samples_num = samples_num;
}

std::vector<std::complex<double>> AudioInOut::MicInput::get_samples() {
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

AudioInOut::SpeakerOutput::SpeakerOutput() {
	this->setup_speaker();
}

void AudioInOut::SpeakerOutput::setup_speaker() {
	this->waveformatx.wFormatTag = WAVE_FORMAT_PCM;
	this->waveformatx.nChannels = 1;
	this->waveformatx.nSamplesPerSec = SAMPLE_RATE;
	this->waveformatx.nAvgBytesPerSec = SAMPLE_RATE * sizeof(short);
	this->waveformatx.nBlockAlign = sizeof(short);
	this->waveformatx.wBitsPerSample = sizeof(short) * 8;
	this->waveformatx.cbSize = 0;
	if (waveOutOpen(&this->handle_wave_out, WAVE_MAPPER, &this->waveformatx, 0, 0, CALLBACK_NULL) != MMSYSERR_NOERROR) {
		Print << U"[-] Error: Failed to open audio device.";
		std::cerr << "[-] Error: Failed to open audio device." << "\n";
	}
}

void AudioInOut::SpeakerOutput::play(std::vector<double> audio_vec) {

	short buffer[SAMPLES_LENGTH];
	for (int i = 0; i < SAMPLES_LENGTH; i++) {
		buffer[i] = static_cast<short>(audio_vec[i] * SHRT_MAX);
	}
	WAVEHDR waveHeader;
	waveHeader.lpData = reinterpret_cast<LPSTR>(buffer);
	waveHeader.dwBufferLength = SAMPLES_LENGTH * sizeof(short);
	waveHeader.dwFlags = 0;
	waveHeader.dwLoops = 0;
	waveHeader.lpNext = nullptr;
	waveHeader.reserved = 0;

	if (waveOutPrepareHeader(this->handle_wave_out, &waveHeader, sizeof(WAVEHDR)) != MMSYSERR_NOERROR) {
		std::cerr << "[-] Failed to prepare audio header." << "\n";
		waveOutClose(this->handle_wave_out);
		return;
	}
	if (waveOutWrite(this->handle_wave_out, &waveHeader, sizeof(WAVEHDR)) != MMSYSERR_NOERROR) {
		std::cerr << "[-] Failed to write audio data." << "\n";
		waveOutUnprepareHeader(this->handle_wave_out, &waveHeader, sizeof(WAVEHDR));
		waveOutClose(this->handle_wave_out);
		return;
	}
	Sleep(10);
	//while (waveOutClose(this->handle_wave_out) == WAVERR_STILLPLAYING);
}


void AudioInOut::SpeakerOutput::close_speaker() {
	waveOutClose(this->handle_wave_out);
}


