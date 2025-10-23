#include "timer.h"

Timer::Timer(float length) : length(length), time(0), timeout(false) {}

void Timer::step(float deltaTime) {
    time += deltaTime;
    if(time >=  length) {
        time -= length;
        timeout = true;
    }
}

bool Timer::isTimeout() const { return timeout ;}
float Timer::getTime() const { return time; }
float Timer::getLength() const { return length; }
void Timer::reset() { time = 0; }
