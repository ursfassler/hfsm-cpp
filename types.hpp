#pragma once

#include <cstddef>


class Event;
class State;
class Transition;


class State
{
  public:
    virtual ~State() = default;

    virtual State* initial() = 0;
    virtual State* parent() const = 0;
    virtual std::size_t transitionCount() const = 0;
    virtual const Transition* transition(std::size_t) const = 0;

    virtual void entry() = 0;
    virtual void exit() = 0;

};

class Transition
{
  public:
    virtual ~Transition() = default;

    virtual State& source() const = 0;
    virtual State& destination() const = 0;
    virtual bool canHandle(const Event* event) const = 0;

    virtual void execute(const Event* event) const = 0;

};
