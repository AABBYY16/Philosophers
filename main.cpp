#include <iostream>
#include <thread>
#include <vector>
#include "philosopher.h"
#include "fork.h"

using namespace std;

int main(int argc, char *argv[])
{
//init philosophers:
    int size = 10;
    vector<Philosopher*> philosophers;
    for(int i=0; i<size; i++){
        int colour = 30+i;
        string colourStr = to_string(colour);
        if(colour > 37)
            colourStr = "3;" + to_string(colour-8);
        philosophers.push_back(new Philosopher(i, colourStr));
    }
//test prints:
    thread *threads = new thread[size];
    for(int i=0; i < size; i++){
        philosophers[i]->print("ciastko");
        threads[i] = new thread(philosophers[i].);
    }
    threa


//ok, do sth cleaver finally:

//    Philosopher *p = new Philosopher(1,"31");
//    cout << "ciaskto program" << endl;
//    p->print("ciastko");
//    Philosopher *p2 = new Philosopher(1,"32");
//    p2->print("ciastko2");
//    Philosopher *p3 = new Philosopher(1,"34");
//    p3->print("ciastko3");
    return 0;
}
