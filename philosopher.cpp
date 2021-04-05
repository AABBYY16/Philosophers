#include "philosopher.h"

Philosopher::Philosopher(int id, shared_ptr<Fork> leftFork, shared_ptr<Fork> rightFork, int timeChunk, int survivarlTime, int tryLockTime, int nearDeathTime, int minTaskTime, int maxTaskTime, string colourRGB, bool noPhilosophersOutputPrints){
    this->id = id;
    this->leftFork = leftFork;
    this->rightFork = rightFork;

    this->timeChunk = timeChunk*1000;
    this->survivarlTime = chrono::milliseconds(survivarlTime * timeChunk);
    this->tryLockTime = chrono::milliseconds(tryLockTime * timeChunk);
    this->nearDeathTime = chrono::milliseconds(nearDeathTime * timeChunk);
    this->minTaskTime = minTaskTime;
    this->maxTaskTime = maxTaskTime;

    this->colourRGB = colourRGB;
    this->noPhilosophersOutputPrints = noPhilosophersOutputPrints;
//    string msg = "Hello, I'm philosopher nr ";
//    this->print(msg.append(to_string(id)));
}

void Philosopher::run(bool *stopCondition){
    lastAte = chrono::system_clock::now();
    while(true){
        if(! this->eating())
            break;
        if(*stopCondition){
            this->state = "FINISHED";
            break;
        }
        if(! this->thinking())
            break;
        if(*stopCondition){
            this->state = "FINISHED";
            break;
        }
    }
}

bool Philosopher::thinking(){
//    this->print("thinking");
    this->state = "THINKING";
    this->delay();
//    this->print("finished thinking");
    //this should never return false as it would mean that thinking time is higher then survival time and philosopher has zero self preservation instinct...he would think himself to the death!
    return this->checkIfAlive();
}

bool Philosopher::eating(){
//    this->print("waiting");
    this->state = "WAITING";
    int attempt = 0;
    while(checkIfAlive()){
        attempt++;
        //lock left fork
        this->leftFork->forkLock.lock();

        //try to lock right fork
        this->leftFork->setReserved(this->id, attempt);
        if(! this->rightFork->forkLock.try_lock_for(this->getTryLockTime())){
            this->leftFork->setFree();
            this->leftFork->forkLock.unlock();
            usleep(this->timeChunk);
            continue;
        }

        //both forks locked, proceed with eating
        this->leftFork->setInUse();
        this->rightFork->setInUse();
//            this->print("eating");
        this->state = "EATING";
        nearDeath = false;
        this->delay();

//            this->print("finished eating");
        //resource releasing order is reversed
        this->rightFork->setFree();
        this->rightFork->forkLock.unlock();
        this->leftFork->setFree();
        this->leftFork->forkLock.unlock();
        lastAte = chrono::system_clock::now();
        break;
    }
    return alive;
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
    if(state == "EATING")
        return 0;
    else if (state == "DEAD")
        return 99;
    else{
        auto duration = new chrono::seconds(chrono::duration_cast<chrono::seconds>(chrono::system_clock::now() - this->lastAte));
        return duration->count();
    }
}

void Philosopher::setState(string state){
    this->state = state;
}

string Philosopher::getProgressBar(){
    return progressBar;
}

int Philosopher::randomTimeChunksAmount(int min, int max){
    return (min + rand() % (max-min));
}
void Philosopher::delay(){
    int length;
    this->progressBar[0]='[';
    this->progressBar[11]=']';
    int sleepTime = this->randomTimeChunksAmount(minTaskTime, maxTaskTime);
    for(int i=0;i<sleepTime;i++){
        length = (int)round(10*i/sleepTime + 1);
        this->progressBar.replace(1, length, string(length, '='));
//        this->progressBar[(int)round(10*i/sleepTime + 1)] = '=';
        usleep(timeChunk);
    }
    this->progressBar = "            ";
}

chrono::milliseconds Philosopher::getTryLockTime(){
    //returns tryLockTime from configuration unless Philosopher is about to die soonner or calls that he is near death. Then it returns time until death
    auto deathAfter = getDeathAfter();
    if(nearDeath || deathAfter < this->tryLockTime) {
        return deathAfter;
    }
    else
        return this->tryLockTime;
}

chrono::milliseconds Philosopher::getDeathAfter(){
    auto now = chrono::system_clock::now();
    auto deathAfter = new chrono::milliseconds(chrono::duration_cast<chrono::milliseconds>(lastAte + this->survivarlTime - now));
    deathAfter->count();
    return *deathAfter;
}

bool Philosopher::checkIfAlive(){
    auto duration = chrono::system_clock::now() - lastAte;
    if(duration > chrono::milliseconds(this->survivarlTime)){
        state = "DEAD";
        alive = false;
        nearDeath = false;
    } else if(duration > (survivarlTime - nearDeathTime)){
        nearDeath = true;
    }
    return alive;
}
