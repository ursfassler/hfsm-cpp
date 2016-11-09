#pragma once

#include <array>

class Event
{
};

class Transition;

class State
{
  public:
    virtual ~State() = default;

    virtual void initialize(State* parent) = 0;
    virtual State* initial() = 0;
    virtual State* parent() const = 0;
    virtual bool isInPathTo(const State* active) = 0;
    virtual const Transition* transitionFor(const Event* event, const State *active) = 0;

};

class Transition
{
  public:
    virtual ~Transition() = default;

    Transition(State* source_, State* destination_, Event* event_);

    State* source{};
    State* destination{};
    Event* event{};

    bool canHandle(const Event* event) const;
    virtual void handle(const Event* event) const;

};

template<std::size_t TransitionCount>
class BaseState :
    public State
{
  public:
    void initialize(State* parent) override
    {
      parentState = parent;
    }

    const Transition* transitionFor(const Event* event, const State *active) override
    {
      for (const Transition *itr : transition)
      {
        if (itr->source->isInPathTo(active) && itr->canHandle(event))
        {
          return itr;
        }
      }

      return nullptr;
    }

    bool isInPathTo(const State* active) override
    {
      if (!active)
      {
        return false;
      }

      if (this == active)
      {
        return true;
      }

      return isInPathTo(active->parent());
    }

    State *parent() const override
    {
      return parentState;
    }

    void addTransition(const Transition* item)
    {
      for (const Transition* &itr : transition)
      {
        if (!itr)
        {
          itr = item;
          return;
        }
      }

      // problem
    }

  private:
    State* parentState{nullptr};
    std::array<const Transition*, TransitionCount> transition{};
};

template<std::size_t TransitionCount, std::size_t SubstateCount>
class CompositeState :
    public BaseState<TransitionCount>
{
  public:
    using BaseState<TransitionCount>::BaseState;

    State* initial() override
    {
      return substate.front()->initial();
    }

    void initialize(State* parent) override
    {
      BaseState<TransitionCount>::initialize(parent);
      if (substate.empty())
      {
        throw std::runtime_error("initial substate has to be defined");
      }
      for (State *itr : substate)
      {
        itr->initialize(this);
      }
    }

    void addState(State* item)
    {
      for (State* &itr : substate)
      {
        if (!itr)
        {
          itr = item;
          return;
        }
      }

      // problem
    }

  private:
    std::array<State*, SubstateCount> substate{};

};

template<std::size_t TransitionCount>
class LeafState :
    public BaseState<TransitionCount>
{
  public:
    State* initial() override
    {
      return this;
    }

};
