#ifndef STATUSPRINTER_H
#define STATUSPRINTER_H

#include <ncurses.h>
#include <chrono>
#include <vector>
#include <unistd.h>
#include <fork.h>
#include <philosopher.h>

struct PhilosopherSummary
{
    int id = 0;
    string state = "";
    int timeSinceEating = 0;
    bool dead = false;
    bool nearDeath = false;
    string nearDeathStr = "";
    int locationX;
    int locationY;
};

struct ForkSummary
{
    int id = 0;
    string state;
    int locationX;
    int locationY;
};

class StatusPrinter
{
private:
    vector<PhilosopherSummary> summariesOfPhilosophers;
    vector<ForkSummary> summariesOfForks;
    vector<Philosopher> *philosophers;
    vector<shared_ptr<Fork>> *forks;
    bool *stopCondition;

    int maxY;
    int maxX;
public:
    StatusPrinter(vector<Philosopher> *philosophers, vector<shared_ptr<Fork>> *forks, bool *stopCondition);
    ~StatusPrinter();

    void printNcurses();
    void summarizePhilosopher(PhilosopherSummary *pilosopherSummary, char *resultChar, unsigned summarySize);
    void summarizeFork(ForkSummary *forkSummary, char *resultChar, unsigned summarySize);
    bool compareAndUpdatePhilosopher(PhilosopherSummary *summary, Philosopher *philosopher);
    bool compareAndUpdateFork(ForkSummary *summary, Fork *philosopher);
    void printOnScreen(char text[], int x, int y);
};

#endif // STATUSPRINTER_H
