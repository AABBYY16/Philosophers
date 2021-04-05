#include "statusprinter.h"

StatusPrinter::StatusPrinter(vector<Philosopher> * philosophers, vector<shared_ptr<Fork>> *forks, bool *stopCondition, int timeChunk)
{
    //commands initing ncurses lbrary
    initscr();
    cbreak();
    noecho();
    nonl();
    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);
    start_color();
    if(has_colors() == FALSE){
        cout << "No colour support within your console!";
        return;
    }
    init_pair(0, COLOR_BLACK, COLOR_WHITE);     //default

    init_pair(1, COLOR_WHITE, COLOR_RED);       //fork in use, philosopher near death
    init_pair(2, COLOR_WHITE, COLOR_GREEN);     //fork free, philosopher eating
    init_pair(3, COLOR_WHITE, COLOR_YELLOW);    //fork reserved, philosopher waiting

    init_pair(4, COLOR_WHITE, COLOR_BLACK);     //philosopher dead
    init_pair(5, COLOR_WHITE, COLOR_BLUE);     //philosopher thinking

    this->philosophers = philosophers;
    this->forks = forks;
    this->stopCondition = stopCondition;
    this->timeChunk = timeChunk*1000;

    getmaxyx(stdscr, maxY, maxX);

    for(unsigned int i=0;i<this->philosophers->size();i++){
        PhilosopherSummary sp = PhilosopherSummary();
        sp.locationY = 2 * i + 1;
        sp.locationX = 0;
        sp.id = philosophers->at(i).getId();
        this->compareAndUpdatePhilosopher(&sp, &philosophers->at(i));
        this->summariesOfPhilosophers.push_back(sp);

        ForkSummary sf = ForkSummary();
        sf.locationY = 2 * i + 1 + 1;
        sf.locationX = 75;
//        sf.locationX = 0;
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
    unsigned summarySize = 75;
    char *summary = new char[summarySize];
    while(! *this->stopCondition){
        for(unsigned int i=0;i<this->philosophers->size() -1;i++){
            summary_philosopher = &summariesOfPhilosophers.at(i);
            if(this->compareAndUpdatePhilosopher(summary_philosopher, &philosophers->at(i))){
                this->summarizePhilosopher(summary_philosopher, summary, summarySize);
                printOnScreen(summary, summary_philosopher->locationX, summary_philosopher->locationY, summary_philosopher->colour);
            }
        }
        for(unsigned int i=0;i<this->forks->size() -1;i++){
            summary_fork = &summariesOfForks.at(i);
            if(this->compareAndUpdateFork(summary_fork, forks->at(i).get())){
                this->summarizeFork(summary_fork, summary, summarySize);
                printOnScreen(summary, summary_fork->locationX, summary_fork->locationY, summary_fork->colour);
                if(i == this->forks->size() - 2)
                    printOnScreen(summary, summary_fork->locationX, 0, summary_fork->colour);
            }
        }
        refresh();
        usleep(this->timeChunk);
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
    str.resize(43, ' ');
    str.insert(43, pilosopherSummary->nearDeathStr);
    str.resize(55, ' ');
    str.insert(55, pilosopherSummary->progressBar);
    str.resize(summarySize, ' ');
    str.copy(resultChar, str.size() + 1);
}

void StatusPrinter::summarizeFork(ForkSummary *forkSummary, char *resultChar, unsigned summarySize){
    string str = "Fork ";
    str.resize(13, ' ');
    str.insert(12, to_string(forkSummary->id));
    str.resize(19, ' ');
    str.insert(19, forkSummary->state);
    str.resize(summarySize, ' ');
    str.copy(resultChar, str.size() + 1);
}

bool StatusPrinter::compareAndUpdatePhilosopher(PhilosopherSummary *summary, Philosopher *philosopher){
    bool isChanged = false;
    if(summary->timeSinceEating != philosopher->getTimeSinceEating()){
        isChanged = true;
        summary->timeSinceEating = philosopher->getTimeSinceEating();
    }
    if(summary->nearDeath != philosopher->getNearDeath()){
        isChanged = true;
        summary->nearDeath = philosopher->getNearDeath();
        if(summary->nearDeath){
            summary->nearDeathStr = "DYING";
            summary->colour = 1;
        }
        else
            summary->nearDeathStr = "";
    }
    if(summary->progressBar.compare(philosopher->getProgressBar())){
        isChanged = true;
        summary->progressBar = philosopher->getProgressBar();
    }
    if(summary->state.compare(philosopher->getState())){
        isChanged = true;
        summary->state = philosopher->getState();
        if(summary->state == "DEAD")
            summary->colour = 4;
        else if(summary->state == "THINKING")
            summary->colour = 5;
        else if(summary->state == "WAITING")
            summary->colour = 3;
        else if(summary->state == "EATING")
            summary->colour = 2;
    }
    return isChanged;
}

bool StatusPrinter::compareAndUpdateFork(ForkSummary *summary, Fork *fork){
    bool isChanged = false;
    if(summary->state.compare(fork->getState())){
        isChanged = true;
        summary->state = fork->getState();
        if(summary->state == "InUse")
            summary->colour = 1;
        else if(summary->state == "Free")
            summary->colour = 2;
        else
            summary->colour = 3;
    }
    return isChanged;
}

void StatusPrinter::printOnScreen(char text[], int x, int y, int colour){
    attron(COLOR_PAIR(colour));
    mvprintw(y, x, text);
    attroff(COLOR_PAIR(colour));
}
