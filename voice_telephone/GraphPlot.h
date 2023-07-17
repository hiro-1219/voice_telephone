#pragma once

#include <Siv3D.hpp>
#include <vector>
#include <complex>

namespace GraphPlot {
	class SpectrumPlot {
	public:
		SpectrumPlot(Vec2 pos, int w);
		void plot(std::vector<double> freq, std::vector<double> spectrum);
	private:
		Vec2 pos;
		int w;
	};
}
