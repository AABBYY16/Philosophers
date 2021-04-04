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
    bool inUse;

public:
    mutex forkLock;

    Fork(int id);

    void aquire();
    void release();

    int getId();

    bool getInUse();
    void setInUse(bool inUse);

//    int getPhilosopherId();
//    void setPhilosopherId(int philosopherId);
};

#endif // FORK_H
