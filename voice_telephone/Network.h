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

		VoicePacket() = default;
	};

	class SendPacket {
	public:
		SendPacket(const char* hostname, int port, int ip_type, int protocol);
		void close_socket();
		void send(VoicePacket voice_packet);
	private:
		WSAData wsa_data;
		SOCKET sock;
		struct sockaddr_in send_addr;
		void setup_send_socket(const char* hostname, int port, int ip_type, int protocol);
		int setup_wsadata();
	};

	/*class ReceivePacket {
	public:
		ReceivePacket();
	private:
		void setup();
	};*/

	union Converter {
		double x_double;
		unsigned long long x_long;
	};

	std::vector<unsigned char> convert_double_to_bytes(std::vector<double> x);
	std::vector<double> convert_bytes_to_double(std::vector<unsigned char> x);
}
