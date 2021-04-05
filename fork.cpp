#include "fork.h"

Fork::Fork(int id){
    this->id = id;
    this->inUse = false;
    this->state = "Free";
}


int Fork::getId(){
    return this->id;
}

string Fork::getState(){
    return this->state;
}

void Fork::setInUse(){
    this->state = "InUse";
}

void Fork::setReserved(int philosopherId, int attempt){
    this->state = "ReservedFor" + to_string(philosopherId) + ":" + to_string(attempt);
}

void Fork::setFree(){
    this->state = "Free";
}

//int Fork::getPhilosopherId(){
//    if(this->inUse)
//        return this->philosopherId;
//    else{
//        cout << "WARNING, fork not in use, returing NULL value";
//        return NULL;
//    }
//}

//void Fork::setPhilosopherId(int philosopherId){
//    this->philosopherId = philosopherId;
//}
