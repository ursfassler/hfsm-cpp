#pragma once

#include "types.hpp"

#include <utility>



class Hfsm final
{
  public:
    Hfsm(State* initial);
    ~Hfsm();

    void handle(const Event* event);

  private:
    State* active{};

    std::pair<const Transition*, State*> firstTransitionTopDown(State* state, const Event *event, const State* active);
    State* handle(State* active, const Event *event);

    State *root() const;
    void callEntry(State* top, State* bottom) const;
    void callExit(State* top, State* bottom) const;


};
