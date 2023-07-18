
#include <Siv3D.hpp>
#include <vector>
#include <cmath>
#include <numeric>
#include <numbers>
#include "LPC.h"

std::vector<double> CodingProcess::get_complex_to_real_vector(std::vector<std::complex<double>> x) {
	int N = x.size();
	std::vector<double> real_vec(N);
	for (int i = 0; i < N; i++) {
		real_vec[i] = x[i].real();
	}
	return real_vec;
}

std::vector<std::complex<double>> CodingProcess::get_real_to_complex_vector(std::vector<double> x) {
	int N = x.size();
	std::vector<std::complex<double>> comp_vec(N);
	for (int i = 0; i < N; i++) {
		comp_vec[i] = std::complex<double>(x[i], 0);
	}
	return comp_vec;
}

CodingProcess::LPCEncrypt::LPCEncrypt(std::vector<double> f, int p) {
	this->p = p;
	this->a = this->calc_lpc_encrypt(f, p);
	this->pe = this->calc_lpc_predict_error(f, this->a);
}

std::vector<double> CodingProcess::LPCEncrypt::calc_lpc_encrypt(std::vector<double> f, int p) {
	std::vector<double> k(p + 1, 0);
	std::vector<double> a(p + 1, 0), tmp_a(p + 1, 0);
	std::vector<double> v = this->calc_autocorrelation(f, p + 1); 
	std::vector<double> ret_a(p);
	double var, w;
	var = v[0]; w = v[1];
	for (int n = 1; n <= p; n++) {
		std::copy(a.begin(), a.end(), tmp_a.begin());
		if (var == 0) k[n] = 0;
		else k[n] = w / var;
		a[n] = -1 * k[n];
		for (int i = 1; i <= n - 1; i++) {
			a[i] = tmp_a[i] - k[n] * tmp_a[n - i];
		}
		var = (1 - k[n] * k[n]) * var;
		if (n + 1 <= p) {
			w = v[n + 1];
			for (int i = 1; i <= n; i++) {
				w += a[i] * v[n + 1 - i];
			}
		}
	}
	std::copy(a.begin() + 1, a.end(), ret_a.begin());
	return ret_a;
}

std::vector<double> CodingProcess::LPCEncrypt::calc_lpc_predict_error(std::vector<double> f, std::vector<double> a) {
	std::vector<double> pe(f.size(), 0);
	for (int i = 0; i < pe.size(); i++) {
		pe[i] = f[i];
		for (int p = 0; p < i; p++) {
			if (a.size() <= p) break;
			else pe[i] -= a[p] * f[i - p - 1];
		}
	}
	return pe;
}


std::vector<double> CodingProcess::LPCEncrypt::calc_autocorrelation(std::vector<double> f, int p) {
	int N = f.size();
	std::vector<double> R(p, 0);
	for (int n = 0; n < p; n++) {
		for (int i = 0; i < N; i++) {
			if(i - n >= 0) R[n] += f[i] * f[i - n];
		}
	}
	return R;
}

std::vector<double> CodingProcess::LPCEncrypt::get_prediction_coefficient() {
	return this->a;
}

std::vector<double> CodingProcess::LPCEncrypt::get_predict_error() {
	return this->pe;
}

CodingProcess::LPCDecrypt::LPCDecrypt(std::vector<double> a, std::vector<double> pe) {
	this->f = this->calc_lpc_decrypt(a, pe);
}

std::vector<double> CodingProcess::LPCDecrypt::calc_lpc_decrypt(std::vector<double> a, std::vector<double> pe) {
	std::vector<double> s(pe.size(), 0);
	for (int i = 0; i < s.size(); i++) {
		s[i] = pe[i];
		for (int p = 0; p <= i; p++) {
			if (a.size() <= p) break;
			else s[i] += a[p] * s[i - p];
		}
	}
	return s;
}

std::vector<double> CodingProcess::LPCDecrypt::get_f_decrypt() {
	return this->f;
}


