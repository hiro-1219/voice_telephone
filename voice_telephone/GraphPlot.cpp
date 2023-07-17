#include <Siv3D.hpp>
#include <vector>
#include <complex>
#include "GraphPlot.h"

GraphPlot::SpectrumPlot::SpectrumPlot(Vec2 pos, int w){
	this->pos = pos;
	this->w = w;
}

void GraphPlot::SpectrumPlot::plot(std::vector<double> freq, std::vector<double> spectrum) {
	LineString specturm_line(freq.size(), this->pos);
	double freq_max = freq[freq.size() - 1];
	for (int i = 0; i < freq.size(); i++) {
		specturm_line[i].set(pos.x + freq[i] * (w / freq_max), pos.y - spectrum[i]);
	}

	specturm_line.draw(2);
}
