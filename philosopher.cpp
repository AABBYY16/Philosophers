#include "philosopher.h"

Philosopher::Philosopher(int id, shared_ptr<Fork> leftFork, shared_ptr<Fork> rightFork, string colourRGB, bool noPhilosophersOutputPrints){
    this->id = id;
    this->leftFork = leftFork;
    this->rightFork = rightFork;
    this->colourRGB = colourRGB;
    this->noPhilosophersOutputPrints = noPhilosophersOutputPrints;
    string msg = "Hello, I'm philosopher nr ";
    this->print(msg.append(to_string(id)));
}

void Philosopher::run(bool *stopCondition){
    while(true){
        this->thinking();
        if(*stopCondition)
            break;
        this->eating();
        if(*stopCondition)
            break;
    }
    this->state = "FINISHED";
}

void Philosopher::thinking(){
    this->print("thinking");
    this->state = "THINKING";
    this->delay();
    this->print("finished thinking");
    this->state = "CHANGING_STATE";
}
void Philosopher::eating(){
    this->print("waiting");
    this->state = "WAITING";
    lock_guard<std::mutex> lockLeft(this->leftFork->forkLock);
    this->leftFork->setInUse(true);
    lock_guard<std::mutex> lockRight(this->rightFork->forkLock);
    this->rightFork->setInUse(true);
    this->print("eating");
    this->state = "EATING";
    this->delay();
    this->print("finished eating");
    this->state = "CHANGING_STATE";
    this->leftFork->setInUse(false);
    this->rightFork->setInUse(false);
}

void Philosopher::print(string text){   //builds wrapped string for thread output
    if(this->noPhilosophersOutputPrints)
        return;
    //set colour
    string str = "\033[";
    str.append(this->colourRGB);
    str.append("m");
    //print thread id
    str.append("Thread ");
    str.append(to_string(this->id));
    str.append(":");
    //print text
    str.append(text);
    //unset colour
    str.append("\033[0m");
    str.append("\n");
    //send to output
    cout << str;
}

int Philosopher::getId(){
    return this->id;
}

string Philosopher::getState(){
    return this->state;
}

int Philosopher::getTimeSinceEating(){
    return this->timeSinceEating;
}

void Philosopher::setState(string state){
    this->state = state;
}

int Philosopher::randomMilisecAmount(int min, int max){
    return (min + rand() % (max-min));
}
void Philosopher::delay(int min, int max){
    int milliseconds = Philosopher::randomMilisecAmount(min, max);
    this_thread::sleep_for(chrono::milliseconds(milliseconds));
}
