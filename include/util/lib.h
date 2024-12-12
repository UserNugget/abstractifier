#ifndef ABSTRACTIFIER_LIB_H
#define ABSTRACTIFIER_LIB_H

#include <iostream>
#include <thread>
#include <chrono>

#define NAME "Abstractifier"
#define TICK_RATE 20
#define ENTITY_CAP 500

#define LOG(message) std::cout << message << '\n';
#define WAIT(time) std::this_thread::sleep_for(std::chrono::milliseconds(time));

uint64_t timeMillis();

#endif
