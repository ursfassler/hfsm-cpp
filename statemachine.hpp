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
    virtual bool isAncestorOf(const State* active) = 0;
    virtual const Transition* transitionFor(const Event* event, const State *active) = 0;
    virtual void addTransition(const Transition* item) = 0;

    virtual void entry() = 0;
    virtual void exit() = 0;

};

class Transition
{
  public:
    virtual ~Transition() = default;

    virtual State* source() const = 0;
    virtual State* destination() const = 0;
    virtual bool canHandle(const Event* event) const = 0;
    virtual void handle(const Event* event) const = 0;

};

class AbstractTransition :
    public Transition
{
  public:
    AbstractTransition(State* source, State* context, State* destination);

    State *source() const override;
    State *destination() const override;

    void handle(const Event* event) const override;

  private:
    State* sourceState{};
    State* destinationState{};

};

class SimpleTransition :
    public AbstractTransition
{
  public:
    SimpleTransition(State* source_, State* context, State* destination_, const Event* event_);

    bool canHandle(const Event* event) const override;

  private:
    State* contextState{};
    const Event* event{};

};

template<std::size_t TransitionCount>
class BaseState :
    public State
{
  public:
    void entry() override
    {
    }

    void exit() override
    {
    }

    void initialize(State* parent) override
    {
      parentState = parent;
    }

    const Transition* transitionFor(const Event* event, const State *active) override
    {
      for (const Transition *itr : transition)
      {
        if (itr->source()->isAncestorOf(active) && itr->canHandle(event))
        {
          return itr;
        }
      }

      return nullptr;
    }

    bool isAncestorOf(const State* active) override
    {
      if (!active)
      {
        return false;
      }

      if (this == active)
      {
        return true;
      }

      return isAncestorOf(active->parent());
    }

    State *parent() const override
    {
      return parentState;
    }

    void addTransition(const Transition* item) override
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

State* handle(State* active, const Event *event);


template<std::size_t TransitionCount, std::size_t SubstateCount>
class Hfsm :
    public CompositeState<TransitionCount, SubstateCount>
{
  public:
    using CompositeState<TransitionCount, SubstateCount>::CompositeState;

    void initialize()
    {
      CompositeState<TransitionCount, SubstateCount>::initialize(nullptr);
      active = CompositeState<TransitionCount, SubstateCount>::initial();
    }

    void handle(const Event* event)
    {
      active = ::handle(active, event);
    }

  private:
    State* active{};

};
