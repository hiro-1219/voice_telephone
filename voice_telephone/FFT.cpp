#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <numbers>
#include <math.h>

#include "FFT.h"

CodingProcess::FFT::FFT(std::vector<std::complex<double>> f, size_t sr) {
	this->F = this->calc_FFT(f);
	this->F_frequency = this->calc_FFT_frequency(sr, f.size());
}

std::vector<std::complex<double>> CodingProcess::FFT::calc_FFT(std::vector<std::complex<double>> f) {
	int N = f.size();
	std::vector<std::complex<double>> F(N), F_even, F_odd, f_even, f_odd;
	if (N == 1) {
		F[0] = f.at(0);
	}
	else {
		for (int i = 0; i < f.size(); i++) {
			if (i % 2 == 0) f_even.push_back(f.at(i));
			else f_odd.push_back(f.at(i));
		}
		F_even = this->calc_FFT(f_even);
		F_odd = this->calc_FFT(f_odd);
		for (int i = 0; i < (int)(N / 2); i++) {
			std::complex<double> w;
			w = this->w_N(N, i);
			F[i] = F_even.at(i) + F_odd.at(i) * w;
			F[i + (int)(N / 2)] = F_even.at(i) - F_odd.at(i) * w;
		}
	}
	return F;
}

std::vector<double> CodingProcess::FFT::calc_FFT_frequency(size_t sr, int N) {
	std::vector<double> freq((int)(N / 2) - 1);
	for (int i = 0; i < (int)(N / 2) - 1; i++) {
		freq[i] = (double)(i * sr) / N;
	}
	return freq;
}

std::complex<double> CodingProcess::FFT::w_N(int N, double k) {
	std::complex<double> w(std::cos(2 * std::numbers::pi * k / N), -1 * std::sin(2 * std::numbers::pi * k / N));
	return w;
}

std::vector<std::complex<double>> CodingProcess::FFT::get_FFT() {
	return this->F;
}

std::vector<double> CodingProcess::FFT::get_FFT_frequency() {
	return this->F_frequency;
}

std::vector<double> CodingProcess::FFT::get_power_spectrum() {
	std::vector<double> power_spectorum(this->F.size());
	for (int i = 0; i < this->F.size(); i++) {
		power_spectorum[i] = (this->F[i] * std::conj(this->F[i])).real();
	}
	return power_spectorum;
}

std::vector<double> CodingProcess::FFT::get_amp_spectrum() {
	std::vector<double> amp_spectrum(this->F.size());
	for (int i = 0; i < this->F.size(); i++) {
		amp_spectrum[i] = std::sqrt(this->F[i].real() * this->F[i].real() + this->F[i].imag() * this->F[i].imag());
	}
	return amp_spectrum;
}

