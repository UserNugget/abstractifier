#ifndef ABSTRACTIFIER_LIB_H
#define ABSTRACTIFIER_LIB_H

#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>

#define NAME "Abstractifier"
#define TICK_RATE 20

#define LOG(message) std::cout << message << '\n';
#define WAIT(time) std::this_thread::sleep_for(std::chrono::milliseconds(time));

uint64_t timeMillis();
int64_t max(int64_t a, int64_t b);

#endif
