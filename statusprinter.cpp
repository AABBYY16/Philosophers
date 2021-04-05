#include "statusprinter.h"

StatusPrinter::StatusPrinter(vector<Philosopher> * philosophers, vector<shared_ptr<Fork>> *forks, bool *stopCondition)
{
    //commands initing ncurses lbrary
    initscr();
    cbreak();
    noecho();
    nonl();
    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);
    if(has_colors() == FALSE){
        cout << "No colour support within your console!";
        return;
    }

    this->philosophers = philosophers;
    this->forks = forks;
    this->stopCondition = stopCondition;

    getmaxyx(stdscr, maxY, maxX);

    for(unsigned int i=0;i<this->philosophers->size();i++){
        PhilosopherSummary sp = PhilosopherSummary();
        sp.locationY = 2 * i + 1;
        sp.locationX = 0;
        sp.id = philosophers->at(i).getId();
        this->compareAndUpdatePhilosopher(&sp, &philosophers->at(i));
        this->summariesOfPhilosophers.push_back(sp);

        ForkSummary sf = ForkSummary();
        sf.locationY = 2 * i;
//        sf.locationX = 60;
        sf.locationX = 0;
        sf.id = philosophers->at(i).getId();
        this->compareAndUpdateFork(&sf, forks->at(i).get());
        this->summariesOfForks.push_back(sf);
    }
}

StatusPrinter::~StatusPrinter(){
    //command closing ncurses lbrary usage
    endwin();
}

void StatusPrinter::printNcurses(){
    PhilosopherSummary *summary_philosopher;
    ForkSummary *summary_fork;
    unsigned summarySize = 50;
    char *summary = new char[summarySize];
    while(! *this->stopCondition){
        for(unsigned int i=0;i<this->philosophers->size() -1;i++){
            summary_philosopher = &summariesOfPhilosophers.at(i);
            if(this->compareAndUpdatePhilosopher(summary_philosopher, &philosophers->at(i))){
                this->summarizePhilosopher(summary_philosopher, summary, summarySize);
                printOnScreen(summary, summary_philosopher->locationX, summary_philosopher->locationY);
            }
        }
        for(unsigned int i=0;i<this->forks->size() -1;i++){
            summary_fork = &summariesOfForks.at(i);
            if(this->compareAndUpdateFork(summary_fork, forks->at(i).get())){
                this->summarizeFork(summary_fork, summary, summarySize);
                printOnScreen(summary, summary_fork->locationX, summary_fork->locationY);
            }
        }
        refresh();
        usleep(100);
    };
    delete[] summary;
}

void StatusPrinter::summarizePhilosopher(PhilosopherSummary *pilosopherSummary, char *resultChar, unsigned summarySize){
    string str = "Philosopher ";
    str.append(to_string(pilosopherSummary->id));
    str.resize(19, ' ');
    str.insert(19, pilosopherSummary->state);
    str.resize(31, ' ');
    str.insert(31, "(LastEat=" + to_string(pilosopherSummary->timeSinceEating) + ")");
    str.resize(summarySize, ' ');
    str.copy(resultChar, str.size() + 1);
}

void StatusPrinter::summarizeFork(ForkSummary *forkSummary, char *resultChar, unsigned summarySize){
    string str = "Fork ";
    str.resize(13, ' ');
    str.insert(12, to_string(forkSummary->id));
    str.resize(19, ' ');
    string status;
    if(forkSummary)
        status = "IN_USE";
    else
        status = "FREE";
    str.insert(19, status);
    str.resize(summarySize, ' ');
    str.copy(resultChar, str.size() + 1);
}

bool StatusPrinter::compareAndUpdatePhilosopher(PhilosopherSummary *summary, Philosopher *philosopher){
    bool isChanged = false;
    if(summary->state.compare(philosopher->getState())){
        isChanged = true;
        summary->state = philosopher->getState();
    }
    if(summary->timeSinceEating != philosopher->getTimeSinceEating()){
        isChanged = true;
        summary->timeSinceEating = philosopher->getTimeSinceEating();
    }
    return isChanged;
}

bool StatusPrinter::compareAndUpdateFork(ForkSummary *summary, Fork *fork){
    bool isChanged = false;
    if(summary->state.compare(fork->getState())){
        isChanged = true;
        summary->state = fork->getState();
    }
    return isChanged;
}

void StatusPrinter::printOnScreen(char text[], int x, int y){
    mvprintw(y, x, text);
}
