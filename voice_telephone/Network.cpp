#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <cmath>
#include <Siv3D.hpp>
#include "Network.h"
#include "constant.h"

#pragma comment(lib, "ws2_32.lib")

VoiceNetwork::SendPacket::SendPacket(const char* hostname, int port, int ip_type, int protocol) {
	setup_wsadata();
	setup_send_socket(hostname, port, ip_type, protocol);
}

int VoiceNetwork::SendPacket::setup_wsadata() {
	if (WSAStartup(MAKEWORD(2, 2), &this->wsa_data) != 0) {
		std::cerr << "[-] Error: WSAStartup()" << "\n";
		return -1;
	}
	return 0;
}

void VoiceNetwork::SendPacket::setup_send_socket(const char* hostname, int port, int ip_type, int protocol) {
	this->sock = socket(ip_type, protocol, 0);
	this->send_addr.sin_family = ip_type;
	this->send_addr.sin_port = htons(port);
	inet_pton(ip_type, hostname, &this->send_addr.sin_addr);
}

void VoiceNetwork::SendPacket::close_socket() {
	closesocket(this->sock);
	WSACleanup();
}

void VoiceNetwork::SendPacket::send(VoiceNetwork::VoicePacket voice_packet){
	std::vector<unsigned char> send_packet_vec = voice_packet.pc;
	send_packet_vec.insert(send_packet_vec.end(), voice_packet.pe.begin(), voice_packet.pe.end());
	Print << U"send: " << LPC_COEFFICIENT_DIM * 8 + voice_packet.pe_length;
	sendto(this->sock, reinterpret_cast<const char*>(send_packet_vec.data()), LPC_COEFFICIENT_DIM * 8 + voice_packet.pe_length, 0, (struct sockaddr*)&this->send_addr, sizeof(this->send_addr));
}

std::vector<unsigned char> VoiceNetwork::convert_double_to_bytes(std::vector<double> x) {
	std::vector<unsigned char> convert;
	VoiceNetwork::Converter conv;
	for (int i = 0; i < x.size(); i++) {
		conv.x_double = x[i];
		for (int j = 0; j < 8; j++) {
			convert.push_back((conv.x_long >> j * 8) & 0xff);
		}
	}
	return convert;
}


std::vector<double> VoiceNetwork::convert_bytes_to_double(std::vector<unsigned char> x) {
	std::vector<double> convert;
	VoiceNetwork::Converter conv;
	for (int i = (int)(x.size() / 8) - 1; i >= 0; i--) {
		unsigned long long tmp = x[i * 8 + 7];
		for (int j = 6; j >= 0; j--) {
			tmp = (tmp << 8) | x[i * 8 + j];
		}
		conv.x_long = tmp;
		convert.push_back(conv.x_double);
	}
	std::reverse(convert.begin(), convert.end());
	return convert;
}
