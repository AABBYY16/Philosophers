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
    int timeChunk=100000;
    chrono::milliseconds tryLockTime = chrono::milliseconds(10 * timeChunk);
    chrono::milliseconds survivarlTime = chrono::milliseconds(90 * timeChunk);
    chrono::milliseconds nearDeathTime = chrono::milliseconds(45 * timeChunk);
    int minTaskTime = 35;
    int maxTaskTime = 45;

    string colourRGB;
    bool noPhilosophersOutputPrints;
    shared_ptr<Fork> leftFork;
    shared_ptr<Fork> rightFork;
private:
//avaliable outsite variables
    int id;
    string state;
    bool nearDeath = false;             //once true, philosopher won't be returning forks anymore as he tries to survive since now!
    bool alive = true;
//inside variables
    chrono::time_point<chrono::system_clock> lastAte;
    chrono::milliseconds timeSinceEating = chrono::milliseconds(0);

public:
    Philosopher(int id, shared_ptr<Fork> leftFork, shared_ptr<Fork> rightFork, int timeChunk, int survivarlTime, int tryLockTime, int nearDeathTime, int minTaskTime, int maxTaskTime, string colourRGB, bool noPhilosophersOutputPrints);
    void run(bool *stopCondition);

    int getId();

    string getState();
    void setState(string state);

    bool getNearDeath(){
        return nearDeath;
    }

    bool checkIfAlive();

    int getTimeSinceEating();

    void print(string text);

private:
    bool thinking();
    bool eating();
    void delay();    //(milliseconds)

    int randomTimeChunksAmount(int min, int max);
    chrono::milliseconds getTryLockTime();
    chrono::milliseconds getDeathAfter();
};

#endif // PHILOSOPHER_H
