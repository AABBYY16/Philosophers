#ifndef PHILOSOPHER_H
#define PHILOSOPHER_H

#include <iostream>

using namespace std;

enum State{
    EATING, THINKING, WAITING
};

class Philosopher{
    State state;
    int id;
    string colourRGB;

public:
    Philosopher(int id, string colourRGB);
    void thinking();
    void eating();

    int getId();

    State getState();
    void setState(State state);

    void print(string text);
};

#endif // PHILOSOPHER_H
