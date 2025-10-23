#pragma once

class Timer {
    float length;
    float time;
    bool timeout;

public:
    Timer(float length);
    void step(float deltaTime);
    bool isTimeout() const;
    float getTime() const;
    float getLength() const;
    void reset();
};