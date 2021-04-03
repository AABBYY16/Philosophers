#ifndef FORK_H
#define FORK_H

#include "philosopher.h"

class Fork {
    int id;
    bool inUse;
    Philosopher *philosopher;

public:

    Fork(int id);

    int getId();

    bool getInUse();
    void setInUse(bool inUse);

    Philosopher * getPhilosopher();
    void setPhilosopher(Philosopher *philosopher);
};

#endif // FORK_H
