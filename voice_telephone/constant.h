#pragma once
#include <Siv3D.hpp>
#include <WinSock2.h>
#include <WS2tcpip.h>

#define SAMPLES_LENGTH 1024
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 600
#define LPC_COEFFICIENT_DIM 20
#define GOLOMB_RICE_DIVISOR 4
#define GOLOMB_RICE_SCALE 1000
#define BACKGROUND_COLOR Palette::White

#define HOST_NAME "127.0.0.1"
#define PORT 12345
#define IP_TYPE AF_INET
#define PROTOCOL SOCK_DGRAM
#define MTU 1500

#define MIC_INDEX 1
#define DEBUG false
