#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>

#define SAMPLE_RATE 48000
#define SAMPLES_LENGTH 1024
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 600
#define LPC_COEFFICIENT_DIM 20
#define GOLOMB_RICE_DIVISOR 4
#define GOLOMB_RICE_SCALE 1000
#define BACKGROUND_COLOR Palette::White

#define HOST_NAME "127.0.0.1"
#define SEND_PORT 12345
#define RECV_PORT 12346
#define RECV_SIZE 7000

#define MIC_INDEX 1
#define BUFFER_NUM 10

#define DEBUG false
