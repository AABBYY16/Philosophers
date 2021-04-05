#include <iostream>
#include <thread>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include "philosopher.h"
#include "fork.h"
#include "additionalfunctions.cpp"
#include "statusprinter.h"

using namespace std;

//WARNING!!! <thread> on linux requires pthread library and visualisation requires lncurses library
//program written in QT and problem has been resolved by adding following entries in project file(.pro):
//    QMAKE_CXXFLAGS += -std=c++0x -pthread
//    LIBS += -pthread
//    LIBS += -lncurses
//console solution: add "-pthread -lncurses" to gcc compiller command

//program stops on ENTER key pressed!

int main(int argc, char *argv[]) {
//program arguments: ${thread_amount} (optional)
    int size = 5;
    if(argc > 1)
        size = atoi(argv[1]) + 1;

//additional arguments
    //chunk of time how long philosopher may keep fork locked(milliseconds)
    int tryLockTime = 1000;
    //time after philosopher dies of hunger(milliseconds)
    int survivarlTime = 9000;
    //time before dying of hunger when philosopher changes a little behaviour of how he acquires fork
    //in theory the best nearDeathTime should be maxEatingtime+tryLockTime(max time to make sure fork will be unlocked unless there are two philosophers next to each other dying of hunger)
    int nearDeathTime = 4500;
    //switches to ugly printing each thread action change created in debugging purpose
    bool noPhilosophersOutputPrints = true;
    //program stops after X secs(0 means to never stop)(to as argument as it was forbiden in exercise description)
    int stopAfter = 0;

//basic definitions:
    srand(time(NULL));

//init forks:
    vector<shared_ptr<Fork>> forks;
    for(int i=0; i < size; i++){
        forks.push_back(make_shared<Fork>(i));
    }

//init philosophers:
    vector<Philosopher> philosophers;
    for(int i=0; i<size; i++){
        int leftForkId;
        if(i == 0)
            leftForkId = size - 1;
        else
            leftForkId = i - 1;
        int rightForkId = i;
        int colour = 30+i;
        string colourStr = to_string(colour);
        //hack for italics font because amount of basic colours is not enough(works for up to 15 threads)
        if(colour > 37)
            colourStr = "3;" + to_string(colour-8);
        philosophers.push_back(Philosopher(i, forks.at(leftForkId), forks.at(rightForkId), survivarlTime, tryLockTime, nearDeathTime, colourStr, noPhilosophersOutputPrints));
    }

////startWatching - printCount
//    bool stopWatchingCondition = false;
//    thread watcher;
//    if(noPhilosophersOutputPrints)
//        watcher = thread(&printCout, &philosophers, &forks, &stopWatchingCondition);


//startWatching - StatusPrinter(ncurses library)
    bool stopWatchingCondition = false;
    thread watcher;
    StatusPrinter statusPrinter = StatusPrinter(&philosophers, &forks, &stopWatchingCondition);
    if(noPhilosophersOutputPrints)
        watcher = thread(&StatusPrinter::printNcurses, &statusPrinter);

//init threads:
    bool stopCondition = false;
    thread *threads = new thread[size];
    for(int i=0; i < size; i++){
        threads[i] = thread(&Philosopher::run, &philosophers[i], &stopCondition);
    }

//terminate program after time/keyPressed:
    bool *detectedEscapeKey = new bool(false);
    thread escapeKeyWatcher = thread(&waitForEscapeKey, detectedEscapeKey, &stopCondition);
    for(int i=0;;i++){
        this_thread::sleep_for(chrono::seconds(1));
        if(*detectedEscapeKey){
            cout << "detectedEscapeKey\n";
            break;
        }
        if(stopAfter == 0){
            continue;
        }
        else
            if(i<stopAfter)
                break;
    }
    stopCondition = true;

//wait for threads and clear program
    for(int i=0; i < size; i++){
        while(! threads[i].joinable())
            usleep(300);
        threads[i].join();
    }
    delete []threads;
    stopWatchingCondition = true;
    escapeKeyWatcher.join();
    delete detectedEscapeKey;
    if(noPhilosophersOutputPrints){
        watcher.join();
    }
    cout << "all threads terminated\n";

//exit
    return 0;
}


