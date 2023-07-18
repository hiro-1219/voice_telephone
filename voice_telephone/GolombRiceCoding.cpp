#include <vector>
#include <cmath>
#include "GolombRiceCoding.h"

CodingProcess::GolombRiceEncode::GolombRiceEncode(std::vector<double> pe, int divisor, double scale){
	this->encoded = this->encode_lpc_predict_error(pe, divisor, scale);
	this->divisor = divisor;
	this->scale = scale;
}

std::vector<unsigned char> CodingProcess::GolombRiceEncode::get_encode() {
	return this->encoded;
}

void CodingProcess::GolombRiceEncode::calc_quotient_remainder(int value, int divisor, int& quotient, int& remainder) {
	quotient = value / divisor;
	remainder = value % divisor;
}

std::vector<unsigned char> CodingProcess::GolombRiceEncode::encode_lpc_predict_error(std::vector<double> pe, int divisor, double scale) {
	std::vector<int> encoded;
	for (double error : pe) {
		int scaled_error = static_cast<int>(std::abs(std::round(error * scale)));
		int quotient, remainder;
		this->calc_quotient_remainder(scaled_error, divisor, quotient, remainder);
		encoded.push_back(error < 0 ? 1 : 0);
		while (quotient > 0) {
			encoded.push_back(0);
			quotient--;
		}
		encoded.push_back(1);
		for (int i = 0; i < divisor - 1; i++) {
			encoded.push_back((remainder >> (divisor - 2 - i)) & 1);
		}
	}
	this->length = encoded.size();
	return this->convert_bits_to_bytes(encoded);
}

std::vector<unsigned char> CodingProcess::GolombRiceEncode::convert_bits_to_bytes(std::vector<int> bits) {
	std::vector<unsigned char> convert;
	if (bits.size() % 8 != 0) {
		for (int i = 0; i < bits.size() % 8; i++) {
			bits.push_back(0);
		}
	}
	for (int i = 0; i < (int)(bits.size() / 8); i++) {
		unsigned char res = 0;
		for (int j = 0; j < 8; j++) {
			res += std::pow(2, j) * bits[i * 8 + j];
		}
		convert.push_back(res);
	}
	return convert;
}

CodingProcess::GolombRiceDecode::GolombRiceDecode(std::vector<unsigned char> encoded, int divisor, double scale) {
	this->decoded = this->decode_lpc_predict_error(encoded, divisor, scale);
	this->divisor = divisor;
	this->scale = scale;
}

std::vector<double> CodingProcess::GolombRiceDecode::get_decode() {
	return this->decoded;
}

int CodingProcess::GolombRiceDecode::golomb_rice_decode(std::vector<int> bits, int divisor, int& start_index) {
	int quotient = 0;
	while (start_index < bits.size() && bits[start_index] == 0) {
		quotient++;
		start_index++;
	}
	start_index++;

	int remainder = 0;
	for (int i = 0; i < divisor - 1; i++) {
		if (start_index < bits.size()) {
			remainder = (remainder << 1) | bits[start_index];
			start_index++;
		}
		else {
			return 0;
		}
	}
	return quotient * divisor + remainder;
}

std::vector<double> CodingProcess::GolombRiceDecode::decode_lpc_predict_error(std::vector<unsigned char> encoded, int divisor, double scale) {
	std::vector<double> pe;
	std::vector<int> encoded_bits = this->convert_bytes_to_bits(encoded);
	int start_index = 0;
	while (start_index < encoded_bits.size()) {
		int sign = encoded_bits[start_index];
		start_index++;
		int decoded_value = this->golomb_rice_decode(encoded_bits, divisor, start_index);
		double error = static_cast<double>(decoded_value) / scale;
		if (sign == 1) {
			error *= -1;
		}
		pe.push_back(error);
	}
	return pe;
}


std::vector<int> CodingProcess::GolombRiceDecode::convert_bytes_to_bits(std::vector<unsigned char> bytes) {
	std::vector<int> convert;
	for (int i = 0; i < bytes.size(); i++) {
		unsigned char res = bytes[i];
		for (int i = 0; i < 8; i++) {
			convert.push_back(res % 2);
			res = (int)(res / 2);
		}
	}
	return convert;
}
