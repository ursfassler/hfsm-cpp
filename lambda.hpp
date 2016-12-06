#pragma once

#include "types.hpp"

#include <functional>
#include <vector>


template<typename E>
class LambdaTransition :
    public Transition<E>
{
  public:
    typedef std::function<void(const E&)> OnEvent;
    typedef std::function<bool(const E&)> CanHandleEvent;

    LambdaTransition(State<E>& source, State<E>& destination, CanHandleEvent canHandleEvent_, OnEvent onEvent_) :
      sourceState{source},
      destinationState{destination},
      canHandleEvent{canHandleEvent_},
      onEvent{onEvent_}
    {
    }

    State<E> &source() const override
    {
      return sourceState;
    }

    State<E> &destination() const override
    {
      return destinationState;
    }

    bool canHandle(const E& event) const override
    {
      return canHandleEvent(event);
    }

    void execute(const E& event) const override
    {
      onEvent(event);
    }

  private:
    State<E>& sourceState;
    State<E>& destinationState;
    CanHandleEvent canHandleEvent;
    OnEvent onEvent;

};


template<typename E>
class LambdaState :
    public State<E>
{
  public:
    typedef std::function<void()> OnActivityChange;


    LambdaState(std::initializer_list<LambdaState<E>*> states_, std::initializer_list<const LambdaTransition<E>*> transitions_, OnActivityChange onEntry_, OnActivityChange onExit_) :
      onEntry{onEntry_},
      onExit{onExit_},
      transitions{transitions_}
    {
      for (auto state : states_)
      {
        state->parentState = this;
      }
      if (states_.size() > 0)
      {
        initialState = *states_.begin();
      }
    }

    void entry() override
    {
      onEntry();
    }

    void exit() override
    {
      onExit();
    }

    std::size_t transitionCount() const override
    {
      return transitions.size();
    }

    const LambdaTransition<E> *transition(std::size_t index) const override
    {
      return transitions[index];
    }

    LambdaState<E> *parent() const override
    {
      return parentState;
    }

    LambdaState<E>* initial() override
    {
      return initialState;
    }

  private:
    OnActivityChange onEntry;
    OnActivityChange onExit;
    LambdaState<E>* parentState{nullptr};
    LambdaState<E>* initialState{this};
    std::vector<const LambdaTransition<E>*> transitions{};

};
