#pragma once

#include "hfsm.hpp"

#include <vector>


template<typename E, typename T>
class SimpleGenericState :
    public State<E>
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

    const Transition<E> *transition(std::size_t index) const override
    {
      return transitions[index];
    }

    void setParent(State<E>* value)
    {
      parentState = value;
    }

    State<E> *parent() const override
    {
      return parentState;
    }

    void addTransition(const Transition<E>* item)
    {
      transitions.push_back(item);
    }

    State<E>* initial() override
    {
      return initialState;
    }

    void addState(SimpleGenericState<E, T> *item)
    {
      if (initialState == this)
      {
        initialState = item;
      }
      item->setParent(this);
    }

  private:
    State<E>* parentState{nullptr};
    State<E>* initialState{this};
    std::vector<const Transition<E>*> transitions{};

};




template<typename E, typename S>
class SimpleGenericTransition :
    public Transition<E>
{
  public:
    SimpleGenericTransition(S& source, S& destination, const E& event_) :
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

    bool canHandle(const E& event) const override
    {
      return event == this->event;
    }

    void execute(const E&) const override
    {
    }

  private:
    S& sourceState;
    S& destinationState;
    const E event{};

};

template<typename E>
class SimpleState;

template<typename E>
class SimpleTransition;

template<typename E>
class SimpleState :
    public SimpleGenericState<E, SimpleTransition<E>>
{
  public:
    using SimpleGenericState<E, SimpleTransition<E>>::SimpleGenericState;
};

template<typename E>
class SimpleTransition :
    public SimpleGenericTransition<E, SimpleState<E>>
{
  public:
    using SimpleGenericTransition<E, SimpleState<E>>::SimpleGenericTransition;
};

