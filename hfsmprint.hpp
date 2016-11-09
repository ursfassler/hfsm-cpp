#pragma once


#include "debug.hpp"

#include <set>
#include <ostream>


class HfsmPrint
{
  public:
    HfsmPrint(State *initial);

    std::set<State*> states;
    std::set<const Transition*> transitions;

    void printStates(std::ostream&);
    void printTransitions(std::ostream&);

  private:
    void addAll(State*);
    void add(State*);

};
