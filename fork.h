#ifndef FORK_H
#define FORK_H

#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

class Fork {
private:
    int id;
//    int philosopherId;
    string state;
    bool inUse;

public:
    timed_mutex forkLock;

    Fork(int id);

    void aquire();
    void release();

    int getId();

    string getState();
    void setInUse();
    void setReserved(int philosopherId);
    void setFree();

//    int getPhilosopherId();
//    void setPhilosopherId(int philosopherId);
};

#endif // FORK_H
