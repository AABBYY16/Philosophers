#ifndef STATUSPRINTER_H
#define STATUSPRINTER_H

#include <ncurses.h>
#include <chrono>
#include <vector>
#include <fork.h>
#include <philosopher.h>

struct PhilosopherSummary
{
    int id = 0;
    string status = "";
    int timeSinceEating = 0;
    bool dead = false;
    int locationX;
    int locationY;
};

class StatusPrinter
{
private:
    vector<PhilosopherSummary> summariesOfPhilosophers;
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
    bool compareAndUpdatePhilosopher(PhilosopherSummary *summary, Philosopher *philosopher);
    void printOnScreen(char text[], int x, int y);
};

#endif // STATUSPRINTER_H
