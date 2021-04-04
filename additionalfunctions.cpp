#include <iostream>
#include <chrono>
#include <thread>
#include "philosopher.h"
#include "fork.h"

static void printCout(vector<Philosopher> *philosophers,vector<shared_ptr<Fork>> *forks, bool *stopCondition){
    string colourRed = "\033[31m";
    string colourGreen = "\033[32m";
    string colourOrange = "\033[33m";
    string colourBlue = "\033[36m";
    string resetColour = "\033[0m";
    string output = "";
    string state;
    while(! *stopCondition){
        system("clear");
        cout << output;
        this_thread::sleep_for(std::chrono::milliseconds(10));
        output = "";
        for(int i=0; i < (int)philosophers->size(); i++){
            output.append("Philosopher ");
            output.append(to_string(philosophers->at(i).getId()));
            output.append(": ");
            state = philosophers->at(i).getState();
            if(state == "EATING"){
                output.append(colourOrange);
                output.append(state);
                output.append(resetColour);
            }
            else if(state == "THINKING"){
                output.append(colourBlue);
                output.append(state);
                output.append(resetColour);
            }
            else
                output.append(state);
            output.append("\n");
            output.append("Fork");
            output.append(to_string(forks->at(i)->getId()));
            output.append(": ");
            if(forks->at(i)->getInUse()){
                output.append(colourRed);
                output.append("in use");
                output.append(resetColour);
            }
            else{
                output.append(colourGreen);
                output.append("free");
                output.append(resetColour);
            }
            output.append("\n");
        }
    }
}

static void waitForEscapeKey(bool *detectedEscapeKey, bool *stopCondition){
    char c;
    while(! *stopCondition){
        this_thread::sleep_for(std::chrono::milliseconds(10));
        c = getchar();
        if( c ){
            *detectedEscapeKey = true;
            break;
        }
    }
}
