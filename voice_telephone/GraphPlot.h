#pragma once

#include <Siv3D.hpp>
#include <vector>
#include <complex>

namespace GraphPlot {
	class SpectrumPlot {
	public:
		SpectrumPlot(Vec2 pos, int w, ColorF color);
		void plot(std::vector<double> freq, std::vector<double> spectrum);
	private:
		Vec2 pos;
		int w;
		ColorF color;
	};

	class AudioPlot {
	public:
		AudioPlot(Vec2 pos, int w, ColorF color);
		void plot(std::vector<double> time, std::vector<double> amp);
	private:
		Vec2 pos;
		int w;
		ColorF color;
	};

	std::vector<double> get_arrange_vec(double start, double end, int N);
}
