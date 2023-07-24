#include <iostream>
#include "Network.h"
#include "constant.h"


int main(void) {
	std::cout << "[+] Start: Voice Telephone server " << "\n";
	VoiceNetwork::SetupWSAStartup setup_wsa = VoiceNetwork::SetupWSAStartup();
	VoiceNetwork::RecvPacket recv_packet = VoiceNetwork::RecvPacket(PORT);

	while (1) {
		recv_packet.recv();
	}

	recv_packet.close_socket();
	setup_wsa.close_wsa();
	return 0;
}
