#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <vector>
#include "constant.h"
#pragma comment(lib, "ws2_32.lib")

namespace VoiceNetwork {
	class VoicePacket {
	public:
		std::vector<unsigned char> pc;
		std::vector<unsigned char> pe;
		int pe_length;
	};

	class SetupWSAStartup {
	public:
		SetupWSAStartup();
		void close_wsa();
	private:
		WSAData wsa_data;
		int setup_wsadata();
	};

	class SendPacket {
	public:
		SendPacket(const char* hostname, int port);
		void close_socket();
		void send(VoicePacket voice_packet);
	private:
		SOCKET sock;
		struct sockaddr_in send_addr;
		void setup_send_socket(const char* hostname, int port);
	};

	class RecvPacket {
	public:
		RecvPacket(int port);
		void close_socket();
		VoicePacket recv();
	private:
		SOCKET sock;
		struct sockaddr_in recv_addr;
		struct sockaddr_in client_addr;
		void setup_recv_socket(int port);
		VoicePacket convert_buffer_to_voice_packet(char* buffer, int buffer_size);
	};

	union Converter {
		double x_double;
		unsigned long long x_long;
	};

	std::vector<unsigned char> convert_double_to_bytes(std::vector<double> x);
	std::vector<double> convert_bytes_to_double(std::vector<unsigned char> x);
}
