#include "philosopher.h"

Philosopher::Philosopher(int id, string colourRGB){
    this->id = id;
    this->colourRGB = colourRGB;
//    this->state = thinking;
    this->state = THINKING;
    string msg = "Hello, I'm philosopher nr ";
    this->print(msg.append(to_string(id)));
}

void Philosopher::thinking(){
    cout << "ciastko thinking" << endl;
}
void Philosopher::eating(){
    cout << "ciastko eating" << endl;
}

void Philosopher::print(string text){
    string setColour = "\033[";
    setColour.append(this->colourRGB);
    setColour.append("m");
    string unsetColour = "\033[0m";
    cout << setColour << text << unsetColour << endl;
}

int Philosopher::getId(){
    return this->id;
}

State Philosopher::getState(){
    return this->state;
}

void Philosopher::setState(State state){
    this->state = state;
}
