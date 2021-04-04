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
