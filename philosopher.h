#ifndef PHILOSOPHER_H
#define PHILOSOPHER_H

#include <iostream>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <vector>
#include "fork.h"

using namespace std;

class Philosopher{
private:
    string state;
    int timeSinceEating = 0;
    int id;
    string colourRGB;
    shared_ptr<Fork> leftFork;
    shared_ptr<Fork> rightFork;
    bool noPhilosophersOutputPrints;

public:
    Philosopher(int id, shared_ptr<Fork> leftFork, shared_ptr<Fork> rightFork, string colourRGB, bool noPhilosophersOutputPrints);
    void run(bool *stopCondition);
    void thinking();
    void eating();

    void waiting();

    int getId();

    string getState();
    int getTimeSinceEating();
    void setState(string state);

    void print(string text);

    static int randomMilisecAmount(int min, int max);

    static void delay(int min=2000, int max=5000);    //(milliseconds)
};

#endif // PHILOSOPHER_H
