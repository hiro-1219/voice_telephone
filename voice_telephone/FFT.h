#pragma once
#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <numbers>
#include <math.h>

namespace CodingProcess {
	class FFT {
	public:
		FFT(std::vector<std::complex<double>> f, size_t sr);
		std::vector<double> get_amp_spectrum();
		std::vector<double> get_power_spectrum();
		std::vector<std::complex<double>> get_FFT();
		std::vector<double> get_FFT_frequency();
	private:
		std::vector<std::complex<double>> F;
		std::vector<double> F_frequency;
		std::complex<double> w_N(int N, double k);
		std::vector<std::complex<double>> calc_FFT(std::vector<std::complex<double>> f);
		std::vector<double> calc_FFT_frequency(size_t sr, int N);
	};
}
