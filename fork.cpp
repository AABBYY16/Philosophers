#include "fork.h"

Fork::Fork(int id){
    this->id = id;
    this->inUse = false;
}


int Fork::getId(){
    return this->id;
}

bool Fork::getInUse(){
    return this->inUse;
}

void Fork::setInUse(bool inUse){
    this->inUse = inUse;
}

Philosopher * Fork::getPhilosopher(){
    return this->philosopher;
}

void Fork::setPhilosopher(Philosopher *philosopher){
    this->philosopher = philosopher;
}
