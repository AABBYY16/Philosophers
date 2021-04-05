#ifndef PHILOSOPHER_H
#define PHILOSOPHER_H

#include <iostream>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include "fork.h"

using namespace std;

class Philosopher{
public:
//configuration
    chrono::milliseconds tryLockTime = chrono::milliseconds(1000);
    chrono::milliseconds survivarlTime = chrono::milliseconds(7000);
    chrono::milliseconds nearDeathTime = chrono::milliseconds(5000);

    string colourRGB;
    bool noPhilosophersOutputPrints;
    shared_ptr<Fork> leftFork;
    shared_ptr<Fork> rightFork;
private:
//avaliable outsite variables
    int id;
    string state;
    bool alive = true;
    bool nearDeath = false;             //once true, philosopher won't be returning forks anymore as he tries to survive since now!
//inside variables
    chrono::time_point<chrono::system_clock> lastAte;
    chrono::milliseconds timeSinceEating = chrono::milliseconds(0);

public:
    Philosopher(int id, shared_ptr<Fork> leftFork, shared_ptr<Fork> rightFork, string colourRGB, bool noPhilosophersOutputPrints);
    void run(bool *stopCondition);

    int getId();

    string getState();
    void setState(string state);

    bool checkIfAlive();

    int getTimeSinceEating();

    void print(string text);

private:
    bool thinking();
    bool eating();
    static void delay(int min=2500, int max=3500);    //(milliseconds)

    static int randomMilisecAmount(int min, int max);
    chrono::milliseconds getTryLockTime();
    chrono::milliseconds getDeathAfter();
};

#endif // PHILOSOPHER_H
