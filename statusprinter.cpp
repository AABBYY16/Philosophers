#include "statusprinter.h"

StatusPrinter::StatusPrinter(vector<Philosopher> * philosophers, vector<shared_ptr<Fork>> *forks, bool *stopCondition)
{
    initscr();                          //command initing ncurses lbrary
    cbreak();
    noecho();
    nonl();
    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);
    if( has_colors() == FALSE ){
        cout << "No colour support within your console!";
        return;
    }

    this->philosophers = philosophers;
    this->forks = forks;
    this->stopCondition = stopCondition;

    getmaxyx(stdscr, maxY, maxX);

    for(unsigned int i=0;i<this->philosophers->size();i++){
        PhilosopherSummary s = PhilosopherSummary();
        this->compareAndUpdatePhilosopher(&s, &philosophers->at(i));
        s.locationY = i;
        s.locationX = 0;
        s.id = philosophers->at(i).getId();
        this->summariesOfPhilosophers.push_back(s);
    }
}

StatusPrinter::~StatusPrinter(){
    endwin();                           //command closing ncurses lbrary usage
}

void StatusPrinter::printNcurses(){
    Philosopher *p;
    PhilosopherSummary *s;
    unsigned summarySize = 50;
    char *summary = new char[summarySize];
    while(! *this->stopCondition){
        for(unsigned int i=0;i<this->philosophers->size() -1;i++){
            p = &philosophers->at(i);
            s = &summariesOfPhilosophers.at(i);
            if(this->compareAndUpdatePhilosopher(s, p)){
                this->summarizePhilosopher(s, summary, summarySize);
                printOnScreen(summary, s->locationX, s->locationY);
            }
        }
        refresh();
        this_thread::sleep_for(std::chrono::milliseconds(10));
    };
    delete[] summary;
}

void StatusPrinter::summarizePhilosopher(PhilosopherSummary *pilosopherSummary, char *resultChar, unsigned summarySize){
    string str = "Philosopher ";
    str.append(to_string(pilosopherSummary->id));
    str.resize(19, ' ');
    str.insert(19, pilosopherSummary->status);
    str.resize(31, ' ');
    str.insert(31, "(LastEat=" + to_string(pilosopherSummary->timeSinceEating) + ")");
    str.resize(summarySize, ' ');
    str.copy(resultChar, str.size() + 1);
}

bool StatusPrinter::compareAndUpdatePhilosopher(PhilosopherSummary *summary, Philosopher *philosopher){
    bool isChanged = false;
    if(summary->status.compare(philosopher->getState())){
        isChanged = true;
        summary->status = philosopher->getState();
    }
    if(summary->timeSinceEating == philosopher->getTimeSinceEating()){
        isChanged = true;
        summary->status = philosopher->getState();
    }
    return isChanged;
}

void StatusPrinter::printOnScreen(char text[], int x, int y){
//    cout << "(" << x << "," << y << ")" << text << endl;
    mvprintw(y, x, text);
}
