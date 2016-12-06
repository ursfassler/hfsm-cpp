#pragma once

#include <cstddef>


template<typename E>
class State;

template<typename E>
class Transition;


template<typename E>
class State
{
  public:
    virtual ~State() = default;

    virtual State* initial() = 0;
    virtual State* parent() const = 0;
    virtual std::size_t transitionCount() const = 0;
    virtual const Transition<E>* transition(std::size_t) const = 0;

    virtual void entry() = 0;
    virtual void exit() = 0;

};

template<typename E>
class Transition
{
  public:
    virtual ~Transition() = default;

    virtual State<E>& source() const = 0;
    virtual State<E>& destination() const = 0;
    virtual bool canHandle(const E& event) const = 0;

    virtual void execute(const E& event) const = 0;

};
