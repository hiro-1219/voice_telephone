#include <Siv3D.hpp>
#include <vector>
#include <complex>
#include "GraphPlot.h"

GraphPlot::SpectrumPlot::SpectrumPlot(Vec2 pos, int w, ColorF color){
	this->pos = pos;
	this->w = w;
	this->color = color;
}

void GraphPlot::SpectrumPlot::plot(std::vector<double> freq, std::vector<double> spectrum) {
	LineString specturm_line(freq.size(), this->pos);
	double freq_max = freq[freq.size() - 1];
	for (int i = 0; i < freq.size(); i++) {
		specturm_line[i].set(this->pos.x + freq[i] * (this->w / freq_max), this->pos.y - spectrum[i]);
	}
	specturm_line.draw(2, this->color);
}

GraphPlot::AudioPlot::AudioPlot(Vec2 pos, int w, ColorF color) {
	this->pos = pos;
	this->w = w;
	this->color = color;
}

void GraphPlot::AudioPlot::plot(std::vector<double> time, std::vector<double> amp) {
	LineString amp_line(time.size(), this->pos);
	double time_max = time[time.size() - 1];
	for (int i = 0; i < time.size(); i++) {
		amp_line[i].set(this->pos.x + time[i] * (this->w / time_max), this->pos.y - amp[i] * 100);
	}
	amp_line.draw(2, this->color);
}

std::vector<double> GraphPlot::get_arrange_vec(double start, double end, int N) {
	double width = end - start;
	std::vector<double> arrange_vec(N, 0);
	for (int i = 0; i < N; i++) {
		arrange_vec[i] = i * (width / N);
	}
	return arrange_vec;
}
