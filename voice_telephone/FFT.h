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
		FFT(std::vector<std::complex<double>> f);
		std::vector<std::complex<double>> convert_amp_spectrum();
		std::vector<std::complex<double>> convert_power_spectrum();
		std::vector<std::complex<double>> get_FFT();
		std::vector<double> get_FFT_frequency();
	private:
		std::vector<std::complex<double>> F;
		std::vector<double> F_frequency;
		std::complex<double> w_N(int N, double k);
	};
}
