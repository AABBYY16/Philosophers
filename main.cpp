#include <iostream>
#include <thread>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include "philosopher.h"
#include "fork.h"
#include "additionalfunctions.cpp"

using namespace std;

//WARNING!!! <thread> on linux requires pthread library.
//program written in QT and problem has been resolved by adding following entries in project file(.pro):
//    QMAKE_CXXFLAGS += -std=c++0x -pthread
//    LIBS += -pthread
//console solution: add "-pthread" to gcc compiller command

//program stops on ENTER key pressed!

int main(int argc, char *argv[]) {
//program arguments: ${thread_amount} (optional)
    int size = 10;
    if(argc > 1)
        size = atoi(argv[1]);

//additional arguments
    //switches to ugly printing each thread action change(usefull in debugging)
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
        philosophers.push_back(Philosopher(i, forks.at(leftForkId), forks.at(rightForkId), colourStr, noPhilosophersOutputPrints));
    }

//startWatching
    bool stopWatchingCondition = false;
    thread *watcher;
    if(noPhilosophersOutputPrints)
        watcher = new thread(&show, &philosophers, &forks, &stopWatchingCondition);

//init threads:
    bool stopCondition = false;
    thread *threads = new thread[size];
    for(int i=0; i < size; i++){
        threads[i] = thread(&Philosopher::run, &philosophers[i], &stopCondition);
    }

//terminate program after time/keyPressed:
    bool *detectedEscapeKey = new bool(false);
    thread *escapeKeyWatcher = new thread(&waitForEscapeKey, detectedEscapeKey, &stopCondition);
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
            usleep('300');
        threads[i].join();
    }
    stopWatchingCondition = true;
    escapeKeyWatcher->join();
    if(noPhilosophersOutputPrints)
        watcher->join();
    delete [] threads;
    cout << "all threads terminated\n";

//exit
    return 0;
}


