#pragma once

#include "hfsm.hpp"

#include <vector>


class Event
{
};


template<typename T>
class SimpleGenericState :
    public State
{
  public:
    void entry() override
    {
    }

    void exit() override
    {
    }

    std::size_t transitionCount() const override
    {
      return transitions.size();
    }

    const Transition *transition(std::size_t index) const override
    {
      return transitions[index];
    }

    void setParent(State* value)
    {
      parentState = value;
    }

    State *parent() const override
    {
      return parentState;
    }

    void addTransition(const Transition* item)
    {
      transitions.push_back(item);
    }

    State* initial() override
    {
      return initialState;
    }

    void addState(SimpleGenericState<T> *item)
    {
      if (initialState == this)
      {
        initialState = item;
      }
      item->setParent(this);
    }

  private:
    State* parentState{nullptr};
    State* initialState{this};
    std::vector<const Transition*> transitions{};

};




template<typename S>
class SimpleGenericTransition :
    public Transition
{
  public:
    SimpleGenericTransition(S& source, S& destination, const Event* event_) :
      sourceState{source},
      destinationState{destination},
      event{event_}
    {
    }

    S &source() const override
    {
      return sourceState;
    }

    S &destination() const override
    {
      return destinationState;
    }

    bool canHandle(const Event* event) const override
    {
      return event == this->event;
    }

    void execute(const Event*) const override
    {
    }

  private:
    S& sourceState;
    S& destinationState;
    const Event* event{};

};

class SimpleState;
class SimpleTransition;

class SimpleState :
    public SimpleGenericState<SimpleTransition>
{
  public:
    using SimpleGenericState<SimpleTransition>::SimpleGenericState;
};

class SimpleTransition :
    public SimpleGenericTransition<SimpleState>
{
  public:
    using SimpleGenericTransition<SimpleState>::SimpleGenericTransition;
};

