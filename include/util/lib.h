#ifndef ABSTRACTIFIER_LIB_H
#define ABSTRACTIFIER_LIB_H

#include <iostream>
#include <thread>
#include <chrono>

#define NAME "Abstractifier"
#define TICK_RATE 20
#define ENTITY_CAP 750

#define LIB_RGBA(r, g, b, a) ((((a) & 0xFF) << 24) | (((b) & 0xFF) << 16) | (((g) & 0xFF) << 8) | (((r) & 0xFF) << 0))
#define LIB_RGB(r, g, b) LIB_RGBA(r, g, b, 255)
#define LOG(message) std::cout << message << '\n';
#define WAIT(time) std::this_thread::sleep_for(std::chrono::milliseconds(time));
#define RND_F ((float) rand() / (float) RAND_MAX)
#define RND_RADIANS radians((RND_F * 5) - 2.5f)

uint64_t timeMillis();

#endif
