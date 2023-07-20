#pragma once

#include<vector>
#include<cmath>

namespace CodingProcess {
	class GolombRiceEncode {
	public:
		GolombRiceEncode(std::vector<double> pe, int divisor, double scale);
		std::vector<unsigned char> get_encode();
	private:
		int divisor;
		int length;
		double scale;
		std::vector<unsigned char> encoded;
		std::vector<unsigned char> encode_lpc_predict_error(std::vector<double> pe, int divisor, double scale);
		void calc_quotient_remainder(int value, int divisor, int& quotient, int& remainder);
		std::vector<unsigned char> convert_bits_to_bytes(std::vector<int> bits);
	};

	class GolombRiceDecode {
	public:
		GolombRiceDecode(std::vector<unsigned char> encoded, int divisor, double scale);
		std::vector<double> get_decode();
	private:
		int divisor;
		double scale;
		std::vector<double> decoded;
		int golomb_rice_decode(std::vector<int> bits, int divisor, int& start_index);
		std::vector<double> decode_lpc_predict_error(std::vector<unsigned char> encoded, int divisor, double scale);
		std::vector<int> convert_bytes_to_bits(std::vector<unsigned char> bytes);
	};	
}
