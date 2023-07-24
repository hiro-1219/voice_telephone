#pragma once
#include <vector>
#include <complex>
#include <cmath>
#include <numeric>
#include <numbers>

namespace CodingProcess{
	std::vector<double> get_complex_to_real_vector(std::vector<std::complex<double>> x);
	std::vector<std::complex<double>> get_real_to_complex_vector(std::vector<double> x);
	class LPCEncrypt {
	public:
		LPCEncrypt(std::vector<double> f, int p);
		std::vector<double> get_prediction_coefficient();
		std::vector<double> get_predict_error();
	private:
		int p;
		std::vector<double> a;
		std::vector<double> pe;
		std::vector<double> calc_lpc_encrypt(std::vector<double> f, int p);
		std::vector<double> calc_lpc_predict_error(std::vector<double> f, std::vector<double> a);
		std::vector<double> calc_autocorrelation(std::vector<double> f, int p);
	};

	class LPCDecrypt {
	public:
		LPCDecrypt(std::vector<double> a, std::vector<double> pe);
		std::vector<double> get_f_decrypt();
	private:
		std::vector<double> f;
		std::vector<double> calc_lpc_decrypt(std::vector<double> a, std::vector<double> pe);
	};
}
