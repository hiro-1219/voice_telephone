#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <cmath>
#include <iostream>
#include "Network.h"
#include "constant.h"

#pragma comment(lib, "ws2_32.lib")

VoiceNetwork::SetupWSAStartup::SetupWSAStartup() {
	this->setup_wsadata();
}

int VoiceNetwork::SetupWSAStartup::setup_wsadata() {
	if (WSAStartup(MAKEWORD(2, 2), &this->wsa_data) != 0) {
		std::cerr << "[-] Error: WSAStartup()" << "\n";
		return -1;
	}
	return 0;
}

void VoiceNetwork::SetupWSAStartup::close_wsa() {
	WSACleanup();
}

VoiceNetwork::SendPacket::SendPacket(const char* hostname, int port) {
	setup_send_socket(hostname, port);
}

void VoiceNetwork::SendPacket::setup_send_socket(const char* hostname, int port) {
	this->sock = socket(AF_INET, SOCK_DGRAM, 0);
	this->send_addr.sin_family = AF_INET;
	this->send_addr.sin_port = htons(port);
	inet_pton(AF_INET, hostname, &this->send_addr.sin_addr);
}

void VoiceNetwork::SendPacket::close_socket() {
	closesocket(this->sock);
}

void VoiceNetwork::SendPacket::send(VoiceNetwork::VoicePacket voice_packet) {
	std::vector<unsigned char> send_packet_vec = voice_packet.pc;
	send_packet_vec.insert(send_packet_vec.end(), voice_packet.pe.begin(), voice_packet.pe.end());
	//send_packet_vec.insert(send_packet_vec.end(), end.begin(), end.end());
	std::cout << "send: " << LPC_COEFFICIENT_DIM * 8 + voice_packet.pe_length + 3 << "\n";
	sendto(this->sock, reinterpret_cast<const char*>(send_packet_vec.data()), LPC_COEFFICIENT_DIM * 8 + voice_packet.pe_length + 3, 0, (struct sockaddr*)&this->send_addr, sizeof(this->send_addr));
}


VoiceNetwork::RecvPacket::RecvPacket(int port) {
	this->setup_recv_socket(port);
}


void VoiceNetwork::RecvPacket::setup_recv_socket(int port) {
	this->sock = socket(AF_INET, SOCK_DGRAM, 0);
	this->recv_addr.sin_family = AF_INET;
	this->recv_addr.sin_port = htons(port);
	this->recv_addr.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(this->sock, (const sockaddr*)&this->recv_addr, sizeof(this->recv_addr)) == SOCKET_ERROR) {
		std::cerr << "[-] Error: Can't bind sock" << "\n";
		closesocket(this->sock);
		WSACleanup();
	}
}


VoiceNetwork::VoicePacket VoiceNetwork::RecvPacket::recv() {
	VoiceNetwork::VoicePacket voice_packet;
	char buffer[RECV_SIZE];
	memset(buffer, 0, RECV_SIZE);
	int buffer_size = recvfrom(this->sock, buffer, RECV_SIZE, 0, NULL, NULL);
	std::vector<unsigned char> buffer_vec(buffer, buffer + sizeof(buffer) - 1);
    std::cout << "recv: " << buffer_size << ":" << buffer[0] << ":" << buffer_vec[0] << "\n";
	return voice_packet;
}

void VoiceNetwork::RecvPacket::close_socket() {
	closesocket(this->sock);
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
